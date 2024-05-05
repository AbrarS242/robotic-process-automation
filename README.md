## Project Overview
This project provides a command-line tool for automating the validation of candidate routines in Robotic Process Automation (RPA). RPA involves automating repetitive tasks by executing sequences of interactions with web and desktop applications. This tool helps identify patterns in executed activities that can be automated, streamlining workflow processes.

## Features
- **Input Data Analysis:** Reads and analyzes input data, ensuring the validity of the input trace.

![Input Data Analysis](https://github.com/AbrarS242/robotic-process-automation/blob/main/example_output/stage_0_output.PNG)

- **Basic Routine Checking:** Performs a basic check of candidate routines by identifying sub-sequences of actions that produce the same cumulative effect as the routine.

![Basic Routine Checking](https://github.com/AbrarS242/robotic-process-automation/blob/main/example_output/stage_1_output.PNG)

- **Advanced Routine Checking:** Extends the checking process to allow identified sub-sequences to modify variable values not set by the candidate routine, ensuring that variables are restored to their original values after execution.

![Advanced Routine Checking](https://github.com/AbrarS242/robotic-process-automation/blob/main/example_output/stage_2_output.PNG)

## Installation
1. Clone the repository to your local machine.
2. Compile the source code using a C compiler (e.g., gcc).

`gcc -o rpa_tool main_program.c` 

3. Ensure that the executable file is accessible from the command line.

## Usage
1. Prepare your input data following format specified below:
2. Launch the program from the command line.
3. Input the data from stdin according to the guidelines provided.

`./rpa_tool < input.txt`

4. The program will process the input and generate the corresponding output, providing information about the input trace and the validation of candidate routines.

## Input Format
- Input should be provided via stdin.
- Start by listing boolean variables, one per line, that must be set to true in the initial state. Use '0' for false and '1' for true.
- Use '#' to denote the end of the initial state section.
- Define actions, one per line, as quintuples: precondition_true precondition_false action_name effect_true effect_false.
- Use '#' to denote the end of the action definition section.
- Encode the trace as a sequence of characters referring to action names.

## Output Format
- The program outputs information about the input trace and the validation of candidate routines.
- It includes a summary of the input trace, its status (valid or invalid), and the validation results for candidate routines.
- ASCII table representations of states and actions are provided.
- The output also indicates the cumulative effect of routines and sub-sequences.

## Note ##
This program was written as an assignment The University of Melbourne. The skeleton code has been written by Artem Polyvyanyy (artem.polyvyanyy@unimelb.edu.au).
