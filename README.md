# Lab 4 – Stack-Based Bytecode Virtual Machine

## Overview
This project implements an assembler that translates human-readable assembly code into executable bytecode, as well as a stack-based bytecode virtual machine (VM).
Arithmetic operations, control flow, memory access, function calls, and execution benchmarking are all supported by the virtual machine.

The virtual machine has a benchmarking mode in addition to core execution that assesses:
* Total time spent executing
* Count of instructions
* Distribution of time among control, ALU, memory, stack, and other operations

For simple analysis and plotting, benchmark results are written to both TXT and CSV formats.

---
# Project Structure
```cpp
.
├── src/
│   ├── vm/
│   │   ├── vm.c
│   │   ├── stack.c
│   │   ├── memory.c
│   │   ├── metrics.c
│   │   └── instruction.c
│   ├── assembler/
│   │   └── assembler.c
│   └── main.c
├── include/
│   ├── vm/
│   └── assembler/
├── tests/
│   ├── easy/
│   ├── medium/
│   ├── hard/
│   └── invalid/
├── benchmarks/
│   ├── benchmark.txt
│   └── benchmark.csv
├── docs/
│   ├── report.pdf
├── Makefile
└── README.md
```
---
# Build Instructions

Compile the VM and assembler using
```bash
make
```
This generates the executable:
```bash
./VM
```
---
# Automated testing
To simplify testing, their is a script file in `tests/run_tests.sh`
This runs all the test cases in tests folder, to run
```bash
cd tests/
chmod +x ./run_tests.sh
./run_tests.sh
```
This writes output in `tests/output.txt`

---
# Command-Line Usage
The VM exection supports 3 execution modes and 2 additional flags
```bash
./VM [-d] [-b] -a  program.asm
./VM [-d] [-b] -e  program.bin
./VM [-d] [-b] -ae program.asm
```
---
## Execution modes

### 1. Assembly Only [-a]
Converts the assembly into bytecode
```bash
./VM -a program.asm
```
This outputs `program.bin`

### 2. Execute only [-e]
Runs the existing bytecode file.
```bash
./VM -e program.bin
```

### 3. Assemble and execute [-ae]
Assembles and executes assembly program.(does not generate .bin file)
```bash
./VM -ae program.asm
```

## Optional Flags

### 1. Debug mode [-d]
Enables verbose debugging output during execution
```bash
./VM -d -ae program.asm
```

### 2. Benchmark mode [-b]
Enables execution benchmarking and writes results to files.
```bash
./VM -b -ae program.asm
```
Benchmark outputs are written to:
```bash
benchmarks/benchmark.txt
benchmarks/benchmark.csv
```

## Example
```bash
./VM -ae -d -b ./tests/medium/test2.asm
```

---
# Benchmarking Metrics
1. Total number of instructions executed
2. Total execution time
3. Time distribution (%) across categories