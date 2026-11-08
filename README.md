# lab04_micro_process_control

This repository contains the report, STM32CubeIDE source code, and Proteus simulation files for Lab 04 of the Microprocessors & Microcontrollers course.

## Directory Structure

```
lab04_micro_process_control/
├── latex/
│   ├── contents/       # LaTeX source files for the report content
│   ├── examples/
│   ├── pictures/       # Images used in the report
│   ├── codespace.sty
│   ├── hcmut-report.cls
│   ├── main.tex
│   ├── main.pdf
│   ├── LICENSE.txt
│   └── README.txt
│── Proteus8/           # contains .hex and .pdsprj files
└── STM32CubeIDE        # contains source files (.c) and header files (.h)
```

## Contents

### `latex/contents/`
The main LaTeX source files where the report itself is written — each exercise's write-up, schematic and answers to the problem (all in `bai_4.tex`).

### `latex/pictures/`
All images used throughout the report — illustrations, schematics, etc.

### `Proteus8/`
Proteus simulation files for each exercise:
- **`.hex`** — compiled hex output used for STM32F103C6.
- **`.pdsprj`** — the Proteus project file containing the circuit schematic and simulation setup.

### `STM32CubeIDE`
Contains `.h` and `.c` files in `lab04_micro_process_control\STM32CubeIDE\lab04\Core\Inc` and `lab04_micro_process_control\STM32CubeIDE\lab04\Core\Src` respectively.
You can open the whole project on your own STM32CubeIDE application, but may encounter directory conflict. Although the app support to rebuild directory to yours, but it not may ensure data integrity.

## Building the Report

The report `Nguyễn Đức Phi Long_2452689_CC04_Lab 04.pdf` is compiled from `latex/main.tex` as the final version.