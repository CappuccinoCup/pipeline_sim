# Pipeline (EZ) Simulator

### 1. Background

Sometimes we need to take an in-depth look at how an instruction is executed and how it affects the CPU states. Given that **pipeline** is widely used in modern CPUs, in this project, we are expected to build an easy pipeline simulator using **ARM** instructions as input.

### 2. Goal

In this project, we will finish a pipeline simulator using C++. Instructions are read from a file along with their latency. The simulator should visualize the execution of these instructions and outputs the change of CPU states, as well as the CPI for this instruction sequence.

### 3. Pipeline Model

The classic pipeline is divided into five stages.

#### IF (Instruction Fetch)

In this stage, we need to fetch the next instruction according to PC. In this project, a **Parser** will read instruction sequences and wrap them into a vector. All we need to do is to get the correct instruction, and then increase PC.

#### ID (Instruction Decode)

For readability, we use the string form of instruction instead of the binary form in "instruction.txt". Thus, in this stage, we needn't actually decode the instruction. All we need to do is to collect the instruction information passed by the **IF** stage.

However, to deal with **data hazards**, we need to support **forwarding**. There are two types of **forwarding**: one is to fetch operand from **EX** stage and the other is from **MEM** stage. (***Note***: to deal with **load-use data hazards**, just forwarding is not enough. Maybe we should do more operations.)

#### EX (Execute)

In this stage, we execute according to the operation types and operands given in the **ID** stage. For arithmetic instructions, we need to calculate the result. For memory access instructions, we need to calculate the memory access address. For branch instructions, we need to calculate the target address.

#### MEM (Memory)

In this stage, we access memory if needed. Also, we need to detect **control hazards** in this stage according to **zero** flag (set in **EX** stage by cmp instructions) and **branch** signal (for simplicity, we only need to check if the **opcode** is a branch instruction).

#### WB (Write Back)

In this stage, we write calculation results back to registers if needed.

After these five pipeline stages, there remain some hazards (**load-use data hazards** and **control hazards**) to be dealt with (normal **data hazards** have been dealt with in **ID** stage using **forwarding**). Thus, we need to perform some additional operations, such as inserting bubbles, changing the value of PC, etc.

***Note***: there is no branch predictor in this pipeline model. In other words, when encountering branch instructions, we always treat it as not taken, even for branch instructions such as **b** and **bl** (to avoid some unnecessary troubles).

### 4. Assumptions

1. There are 16 32-bit registers: general purpose registers (r0 - r13), stack pointer (sp), link register (lr), and program counter (pc).
2. Total memory space is 6000 bytes. Top address [5001, 6000] is preserved. Stack decreases from 5000, while heap increases from 0.
3. PC starts from 0x0. Memory allocation is defined using ".space" followed by the allocated size (bytes). 
4. Compare (cmp) and the following Branch (bge or bne) are considered to be one instruction, like "cmp reg1, reg2 bge label".
5. Input instruction sequence is taken from "instruction.txt" while latency is taken from "latency.txt".

### 5. Compile and Run

Open the terminal, cd to the "pipeline_sim" directory, and execute the following commands:

```bash
# Compile pipeline_sim
g++ -std=c++11 ./src/pipeline_sim.cpp ./src/parser.cpp -o pipeline_sim

# The input instructions file is preset to "./instruction.txt"
# The input latency file is preset to "./latency.txt"
./pipeline_sim
```

The simulation result will be dumped into "result.txt". It shows which instructions are executed at each stage of the pipeline for each clock cycle, as well as the value of each register at the end of this cycle. For example: 

```bash
# "instruction.txt" is the same content as "testcases/EZCall.txt"

# ...

# In this cycle, pipeline is executing as following

======= WB =======			# Write back the result of a "mov" instr (pc: 0x04)
mov
======= MEM ======			# Access memory for a "mov" instr (pc: 0x08)
mov
======= EX =======			# Execute for a "cmp & bge" instr (pc: 0x0c)
cmp & bge
======= ID =======			# Decode a instr (turn out to be a "bl" instr, pc: 0x10)
bl
======= IF =======			# Fetch a instr (turn out to be a "b" instr, pc: 0x14)
b

# After the above execution, register state is:

reg[0]    = 1         	reg[1]    = 2         	reg[2]    = 0         	reg[3]    = 0         	
reg[4]    = 0         	reg[5]    = 0         	reg[6]    = 0         	reg[7]    = 0         	
reg[8]    = 0         	reg[9]    = 0         	reg[10]   = 0         	reg[11]   = 0         	
reg[12]   = 0         	reg[13]   = 3000      	reg[14]   = 0x0       	reg[15]   = 0x18      	

# Instruction executed so far:
Instr N: 2
# Latency (clock cycle) so far:
Latency: 7

# ...

# CPI = Latency / Instr N
CPI: 3.2
```

***Note***: we provide a sample program "pipeline_sim_ref" for reference. We also give 5 sample instruction sequences in "testcases". You can replace the contents of "instruction.txt" with any of these test cases, and then run the sample program. Observe the test case and the corresponding "result_ref.txt" and you can get more understanding.

### 6. Project Structure

```
|
|- src
| |- arm_instr.h		// Some definitions for ARM.
| |- parser.cpp			// Latency and instructions parser.
| |- parse.h			// Header file for parser.
| |- pipeline_sim.cpp	// Pipeline simulator.
| |- pipeline_sim.h		// Header file for simulator.
|
|- testcases			// You can copy the content of any
| |						// test case to "instruction.txt"
| |- ControlHazard.txt
| |- DataHazard.txt
| |- EZCall.txt
| |- Recursion.txt
| |- SumOfNumbers.txt
|
|- instruction.txt		// Instruction file. Modify it
|						// to test more instruction sequences.
|
|- latency.txt			// Latency file.
|						// Don't need to modify.
|
|- pipeline_sim_ref		// Sample program for reference.
|
```

### 7. Supported Instructions

1. add rd, reg1, reg2
2. add rd, reg1, #imm
3. add sp, sp, #imm
4. sub rd, reg1, reg2
5. sub rd, reg1, #imm
6. sub sp, sp, #imm
7. mul rd, reg1, reg2
8. mul rd, reg1, #imm
9. mov rd, reg1
10. mov rd, #imm
11. mov pc, lr
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
