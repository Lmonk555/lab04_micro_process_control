# lab03_micro_process_control

This repository contains the report, STM32CubeIDE source code, the require video, and Proteus simulation files for Lab 03 of the Microprocessors & Microcontrollers course.

## Directory Structure

```
lab03_micro_process_control/
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
The main LaTeX source files where the report itself is written — each exercise's write-up, schematic and answers to the problem (all in `bai_3.tex`).

### `latex/pictures/`
All images used throughout the report — illustrations, schematics, etc.

### `Proteus8/`
Proteus simulation files for each exercise:
- **`.hex`** — compiled hex output used for STM32F103C6.
- **`.pdsprj`** — the Proteus project file containing the circuit schematic and simulation setup.

### `STM32CubeIDE`
Contains `.h` and `.c` files in `lab03_micro_process_control\STM32CubeIDE\lab_03\Core\Inc` and `lab03_micro_process_control\STM32CubeIDE\lab_03\Core\Src` respectively.
You can open the whole project on your own STM32CubeIDE application, but may encounter directory conflict. Although the app support to rebuild directory to yours, but it not may ensure data integrity.

In this lab session, we just use one `.pdsprj` and one `.hex` file as required.

The required video is store in Google Drive, which is also mentioned in the report: https://drive.google.com/file/d/1MUBsJ7rMVX56908bq_3bAQ5zY5j9nRom/view?usp=sharing
## Building the Report

The report `Nguyễn Đức Phi Long_2452689_CC04_Lab 03.pdf` is compiled from `latex/main.tex` as the final version.