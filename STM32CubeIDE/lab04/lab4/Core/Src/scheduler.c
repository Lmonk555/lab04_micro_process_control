#include <string.h>
#include "main.h"
#include "scheduler.h"





typedef struct
{
	void (*pTask)(void);	// Pointer to the task (must be a "void (void)" function)
	uint32_t Delay;			// ticks AFTER the previous task in the list
	uint32_t Period;		// ticks between runs, 0 = one-shot
	uint8_t  RunMe;			// 1 when the task is due to execute (only used for the head)
	uint32_t TaskID;		// ID returned by SCH_Add_Task
} sTask;

static sTask    SCH_tasks_G[SCH_MAX_TASKS];		// [0] = next task due
static uint8_t  SCH_count = 0;					// tasks currently in the list
static uint32_t newTaskID = 0;
static volatile uint32_t tickCount = 0;			// ticks since SCH_Init

uint8_t Error_code_G = 0;





static uint32_t Get_New_Task_ID(void)
{
    if (++newTaskID == NO_TASK_ID)		// never hand out ID 0
    	newTaskID++;
    return newTaskID;
}


void SCH_Init(void)
{
    memset(SCH_tasks_G, 0, sizeof(SCH_tasks_G));
    SCH_count = 0;
    newTaskID = 0;
    tickCount = 0;
    Error_code_G = 0;
}


/* Called from the timer ISR every tick. It does NOT run any task, only counts down the head and set RunMe flag. */
void SCH_Update(void)
{
    tickCount++;
    if (SCH_count == 0) return;									// no task in SCH_tasks_G[], return
    if (SCH_tasks_G[0].Delay > 0) SCH_tasks_G[0].Delay--;		// ticks--
    if (SCH_tasks_G[0].Delay == 0) SCH_tasks_G[0].RunMe = 1;	// ticks == 0 --> set it prepare to run
}


/* Sorted insert, O(N). Equal due times keep FIFO order ('<=' below).
 * Interrupts are disabled while the array is shifted, otherwise the ISR
 * (SCH_Update) could read a half-moved head. */
static uint32_t Insert(void (*pF)(void), uint32_t delay, uint32_t period, uint32_t id)
{
    __disable_irq();
    if (SCH_count >= SCH_MAX_TASKS)				// task list is full
    {
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        __enable_irq();
        return NO_TASK_ID;
    }


    // 1. find the position: skip every task due before (or with) this one
    uint8_t  idx = 0;
    uint32_t sum = 0;                           // absolute due time of tasks[idx-1]
    while (idx < SCH_count && sum + SCH_tasks_G[idx].Delay <= delay)
    {
        sum += SCH_tasks_G[idx].Delay;
        idx++;
    }


    // 2. open a gap (shift whole records, RunMe included)
    for (uint8_t i = SCH_count; i > idx; i--)
        SCH_tasks_G[i] = SCH_tasks_G[i - 1];


    // 3. fill the new node; its Delay is relative to the previous task
    SCH_tasks_G[idx].pTask  = pF;
    SCH_tasks_G[idx].Delay  = delay - sum;
    SCH_tasks_G[idx].Period = period;
    SCH_tasks_G[idx].TaskID = id;
    SCH_tasks_G[idx].RunMe  = (idx == 0 && SCH_tasks_G[idx].Delay == 0);


    // 4. the successor now waits relative to the new node
    if (idx < SCH_count)
        SCH_tasks_G[idx + 1].Delay -= SCH_tasks_G[idx].Delay;
    SCH_count++;
    __enable_irq();
    return id;
}


uint32_t SCH_Add_Task(void (*pFunction)(), uint32_t DELAY, uint32_t PERIOD)
{
    // returns the task ID (NO_TASK_ID if the list is full)
    return Insert(pFunction, DELAY, PERIOD, Get_New_Task_ID());
}


/* Remove the task at id. The successor inherits its Delay, so the
 * tasks behind it keep their absolute due times. (caller disables interrupts) */
static void Remove_At(uint8_t id)
{
    if (id + 1 < SCH_count)
    {
        SCH_tasks_G[id + 1].Delay += SCH_tasks_G[id].Delay;
        SCH_tasks_G[id + 1].RunMe = (id == 0 && SCH_tasks_G[id + 1].Delay == 0);
    }
    for (uint8_t j = id; j + 1 < SCH_count; j++)
        SCH_tasks_G[j] = SCH_tasks_G[j + 1];
    SCH_count--;
    memset(&SCH_tasks_G[SCH_count], 0, sizeof(sTask));
}


uint8_t SCH_Delete_Task(uint32_t taskID)
{
    uint8_t Return_code = RETURN_ERROR;
    if (taskID != NO_TASK_ID)
    {
        __disable_irq();
        for (uint8_t i = 0; i < SCH_count; i++)
        {
            if (SCH_tasks_G[i].TaskID == taskID)
            {
                Remove_At(i);
                Return_code = RETURN_NORMAL;
                break;
            }
        }
        __enable_irq();
    }
    if (Return_code == RETURN_ERROR)
    {
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;    // no such task
    }
    return Return_code;
}


void SCH_Dispatch_Tasks(void)
{
    // Dispatches (runs) every task that is due
    while (SCH_count > 0 && SCH_tasks_G[0].RunMe)
    {
        sTask t = SCH_tasks_G[0];							// copy: the list may change
        __disable_irq();
        Remove_At(0);										// take it out of the list
        __enable_irq();


        // Periodic tasks run again: re-arm BEFORE running, so ticks keep
        // counting while the task runs and a task may delete itself.
        if (t.Period > 0)
            Insert(t.pTask, t.Period, t.Period, t.TaskID);	// keeps its ID
        (*t.pTask)();										// run the task
    }
}


uint32_t get_time(void)
{
    return tickCount * TICK_MS;
}
