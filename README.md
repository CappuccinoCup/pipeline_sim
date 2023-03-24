## Problem Statement
There is a need to visualize what is going at lower layer of abstraction when we execute the high level or low level program on a Von Neumann machine. The project aims to build an ARM language simulator to visualize the execution of ARM language(low level) instructions. As in case of advanced processors the concept of pipelining are used, the simulator should be able to visualize the execution of instructions using pipelining concept. The simulator should simulate the recursive function, if statements and for loops statement.

## Abstract of the Development Phase
We developed the project using c++ and the concept of pipelining. Instruction are read from a file along with the latency of the instructions. The simulator was able to visualize the execution of the instruction and displayed the execution on the terminal.

## Command to Run Program
1. Open Terminal and cd to ./src
2. g++ -std=c++11 ./src/pipeline_sim.cpp ./src/parser.cpp -o pipeline_sim
3. ./pipeline_sim
*input to simulator is given through ./src/latency.txt and ./src/input.txt*

## Assumption
1. Cmp and Branch(bge and bne) are considered to be one instruction.
2. Pseudo Memory is of size 5000,label is assigned memory according to .data section.
3. Input is taken from in.txt while latency data is taken from latency.txt 

## GUI
1. Different color are used for every pipeline stage.
2. Instruction in particular stage is shown just below it in white color.
3. If there is no instruction in particular stage then there will be nothing written below it.
4. When instruction is flushed then it is shown by printing **flush** in that stage.
5. Whereas when we need stall to remove hazard **stall** will be written below that stage.

## Instruction Set
1. add rd, rop1, rop2
2. add rd, rop1, #imm
3. add sp, sp, #imm
4. sub rd, rop1, rop2
5. sub rd, rop1, #imm
6. sub sp, sp, #imm
7. mul rd, rop1, rop2
8. mul rd, rop1, #imm
9. mov rd, rop1
10. mov pc, lr
11. mov rd, #imm
12. ldr rd, [rop1, #imm]
13. ldr rd, [rop1]
14. ldr rd, =label
15. ldr lr, [sp, #imm]
16. ldr rd, [sp, #imm]
17. str rd, [rop1, #imm]
18. str rd, [rop1]
19. str lr, [sp, #imm]
20. str rd, [sp, #imm]
21. cmp rop1, rop2 bge label
22. cmp rop1, rop2 bne label
23. bl procedureLabel
24. b label
25. Exit
