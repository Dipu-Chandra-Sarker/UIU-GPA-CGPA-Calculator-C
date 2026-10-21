# GPA & CGPA Calculator System in C

A console program in C that calculates GPA and CGPA using the United International University (UIU) grading policy, including retake courses.

Made for the Structured Programming Language Laboratory course at UIU (Summer 2024).

## Features

- GPA calculation from credits and grades
- CGPA with retake courses or trimester-wise
- Edit inputs and see results instantly
- Save and load results from text files
- Grading policy, formulas and help page

## Files

| File | What it is |
|---|---|
| `GPA_CGPA_Calculator.c` | Source code |
| `GPA_CGPA_Calculator.exe` | Ready-to-run program for Windows |
| `README.md` | This guide |

## How to run

### Option 1: Run the .exe (easiest)

1. Download `GPA_CGPA_Calculator.exe`.
2. Double-click it.
3. If Windows shows a warning, click **More info**, then **Run anyway**.

### Option 2: Run in Code::Blocks

1. Download `GPA_CGPA_Calculator.c`.
2. Open Code::Blocks and go to **File → Open**, then choose the `.c` file.
3. Press **F9** (Build and Run).
4. The program opens in a console window.

## How to use

1. Choose an option from the main menu by typing its number.
2. Enter the credits and grades when asked. Grades can be small or capital letters (b+ or B+).
3. After the result, you can change any input or save the result to a `.txt` file.
4. Use **Load saved record** to open a saved result later.

## Grading policy

| Grade | Point | Grade | Point |
|---|---|---|---|
| A  | 4.00 | C+ | 2.33 |
| A- | 3.67 | C  | 2.00 |
| B+ | 3.33 | C- | 1.67 |
| B  | 3.00 | D+ | 1.33 |
| B- | 2.67 | D  | 1.00 |
|    |      | F  | 0.00 |

## Formulas

- GPA = Σ (grade point × credit) / total credits
- CGPA (trimester-wise) = Σ (trimester GPA × trimester credits) / total credits
- CGPA (with retake) = (old CGPA × old credits + new points) / total credits, where a retake's new grade replaces the old one and its credits are not counted again

## Author

Dipu Chandra Sarker, Department of EEE, United International University
