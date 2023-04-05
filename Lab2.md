# Architecture Labs: Pipeline - Lab2

### 0. Background

Sometimes we need to take an in-depth look at how an instruction is executed and how it affects the CPU states. Given that ***pipeline*** is widely used in modern CPUs, in Lab2, you will build a pipeline simulator using ***ARM*** instructions as input. Here we go.

### 1. Goal

In this lab, you will finish a pipeline simulator using C++. Instructions are read from a file ("input.txt" below) along with their latency ("latency.txt" below). The simulator should visualize the execution of these instructions and outputs the change of CPU states.

### 2. Compile and Run

Open the terminal, cd to the "pipeline_sim" directory, and execute the following commands:

```bash
# Compile pipeline_sim
g++ -std=c++11 ./src/pipeline_sim.cpp ./src/parser.cpp -o pipeline_sim

# The input instructions file is set default as "./instruction.txt"
# The input latency file is set default as "./latency.txt"
./pipeline_sim
```

The first time you run will see nothing changed in the CPU states after each instruction is executed, like this:

```bash

```

It's your work to complete the pipeline simulator.

### 3. TODO



### 4. Assumptions

1. Compare (cmp) and the following Branch (bge and bne) are considered to be one instruction, like "cmp reg1, reg2 bge/bne label"
2. Pseudo Memory is of size 5000 bytes, the label is assigned memory according to the .data section.
3. Input instruction is taken from input.txt while latency is taken from latency.txt.

### 5. Instruction Set

1. add rd, reg1, reg2
2. add rd, reg1, #imm
3. add sp, sp, #imm
4. sub rd, reg1, reg2
5. sub rd, reg1, #imm
6. sub sp, sp, #imm
7. mul rd, reg1, reg2
8. mul rd, reg1, #imm
9. mov rd, reg1
10. mov pc, lr
11. mov rd, #imm
12. ldr rd, =label
13. ldr rd, [reg1]
14. ldr rd, [reg1, #imm]
15. ldr rd, [sp, #imm]
16. ldr lr, [sp, #imm]
17. str rd, [reg1]
18. str rd, [reg1, #imm]
19. str rd, [sp, #imm]
20. str lr, [sp, #imm]
21. cmp reg1, reg2 bne label
22. cmp reg1, #imm bne label
23. cmp reg1, reg2 bge label
24. cmp reg1, #imm bge label
25. bl procedureLabel
26. b label
27. Exit

### 6. Submission

