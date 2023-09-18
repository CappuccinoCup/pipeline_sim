import os

ori_instruction = """main:
0x000:  mov r0, #1
0x004:  mov r1, #2
0x008:  mov r2, #4
l1:
0x00c:  cmp r0, r2
        bge l2
0x010:  bl  sum             @ Call.
0x014:  b   l1
sum:
0x018:  add r0, r0, r1
0x01c:  mov pc, lr          @ Return.
l2:
0x020:  sub r0, r0, r1
0x024:  Exit
"""

ori_latency = """# The number of clock cycles required to execute each instruction
add = 1;
sub = 1;
mul = 5;
mov = 1;
ldr = 20;
ldr_pseudo = 1;
str = 20;
cmp = 1;
bne = 2;
bge = 2;
bl = 2;
b = 2;
"""

test_case = [
    {
        "name": "ControlHazard",
        "content": """main:
0x000:  mov r1, #2
0x004:  mov r2, #1
l1:
0x008:  sub r1, r1, #1
0x00c:  cmp r1, #0          @ Control hazard. Twice.
        bge l1
0x010:  mov r3, #3
0x014:  b  l2               @ Control hazard. Once.
0x018:  add r3, r3, r1
0x01c:  sub r3, r3, r2
l2:
0x020:  mov r3, #2
0x024:  Exit
""",
        "answer": {
            "regs": [0, -1, 1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 0x0, 0x28],
            "instr_num": 11,
            "latency": 29
        }
    },
    {
        "name": "DataHazard",
        "content": """main:
0x000:  mov r6, #4
0x004:  mov r7, #0
0x008:  str r6, [r7, #4]    @ Data hazard on reg[7]. Deal with EX forward.
0x00c:  ldr r1, [r6]        @ Data hazard on mem[4]. No extra operation needed.
0x010:  str r7, [r1]        @ Data hazard on reg[1]. Deal with MEM forward and one BUBBLE.
0x014:  mov r2, #4
0x018:  ldr r2, [r2]        @ Data hazard on reg[2]. Deal with EX forward.
0x01c:  mul r2, r2, #2      @ Data hazard on reg[2]. Deal with MEM forward and one BUBBLE.
0x020:  add r3, r2, r1      @ Data hazard on reg[2]. Deal with EX forward.
0x024:  Exit
""",
        "answer": {
            "regs": [0, 4, 0, 4, 0, 0, 4, 0, 0, 0, 0, 0, 0, 3000, 0x0, 0x28],
            "instr_num": 9,
            "latency": 96
        }
    },
    {
        "name": "EZCall",
        "content": """main:
0x000:  mov r0, #1
0x004:  mov r1, #2
0x008:  mov r2, #4
l1:
0x00c:  cmp r0, r2
        bge l2
0x010:  bl  sum             @ Call.
0x014:  b   l1
sum:
0x018:  add r0, r0, r1
0x01c:  mov pc, lr          @ Return.
l2:
0x020:  sub r0, r0, r1
0x024:  Exit
""",
        "answer": {
            "regs": [3, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 0x14, 0x28],
            "instr_num": 15,
            "latency": 48
        }
    },
    {
        "name": "Recursion",
        "content": """main:
0x000:  mov r0, #6
0x004:  bl  fact
0x008:  b   l2
fact:
0x00c:  sub sp, sp, #8
0x010:  str lr, [sp, #4]
0x014:  str r0, [sp, #0]
0x018:  cmp r0, #1
        bne l1
0x01c:  mov r0, #1
0x020:  add sp, sp, #8
0x024:  mov pc, lr
l1:
0x028:  sub r0, r0, #1
0x02c:  bl  fact
0x030:  mov r12, r0
0x034:  ldr r0, [sp, #0]
0x038:  ldr lr, [sp, #4]
0x03c:  add sp, sp, #8
0x040:  mov r1, r0
0x044:  mul r0, r1, r12
0x048:  mov pc, lr
l2:
0x04c:  Exit
""",
        "answer": {
            "regs": [720, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 120, 3000, 0x8, 0x50],
            "instr_num": 75,
            "latency": 585
        }
    },
    {
        "name": "SumOfNumbers",
        "content": """    .text

@ StoreIntegers - Stores integers (in this case, 1 to 100) at a destination Address
@ r3 			- Destination Address at which the integers are stored 
@ r2 			- Integers (in this case, starting from 1 to 100)
@ r1 			- Loop counter (in this case, going from 100 to 1)

0x000:  mov r1,	#100		@Moves constant 100 to register r1
0x004:  mov r2, #1
0x008:  ldr r3, =AA		    @Loads address of label AA in register r3

StoreIntegers:
0x00c:  str r2, [r3]		@ Stores the value of register r2 to address stored in register r3
0x010:  add r3, r3, #4	 	@ Update Destination Address(4 bytes)
0x014:  add r2, r2, #1 		@ Update Integer value (add 1)
0x018:  sub r1, r1, #1 		@ Update Loop counter (sub 1)
0x01c:  cmp r1, #0 	        @ Compare Loop counter with 0
        bne StoreIntegers	@ If Loop counter != 0, keep looping

@LoadAddIntegers- Loads integers (in this case, 1 to 100) from a source Address and sums them up.
@ r4 			- Sum register
@ r3 			- Source Address at which the integers are loaded from 
@ r2 			- Integers (in this case, starting from 1 to 100)
@ r1 			- Loop counter (in this case, going from 100 to 1)

0x020:  mov r1, #100
0x024:  mov r4, #0
0x028:  ldr r3, =AA

LoadAddIntegers:
0x02c:  ldr r2, [r3]     	@ Loads integers from address stored in register r3 to register r2
0x030:  add r4, r4, r2      @ Update Sum (= Previous sum + current value)
0x034:  add r3, r3, #4   	@ Update Source Address (4 bytes) 
0x038:  sub r1, r1, #1   	@ Update Loop counter (sub 1)
0x03c:  cmp r1, #0        	@ Compare Loop counter with 0
	    bne LoadAddIntegers	@ If Loop counter != 0, keep looping 

0x040:  Exit		        @ Stops the program

	.data				@ Do a memory allocation for 400 words (each 4 bytes)
AA:
    .space 400
    .end
""",
        "answer": {
            "regs": [0, 0, 100, 400, 5050, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 0x0, 0x44],
            "instr_num": 1006,
            "latency": 5745
        }
    },
    {
        "name": "Extra_1",
        "content": """main:
0x000:  mov r0, #1
0x004:  mov r1, #1
0x008:  mov r10, #0
l1:
0x00c:  cmp r10, #10
        bge l2
fib:
0x010:  mov r9, r1
0x014:  mov r1, r0
0x018:  add r0, r0, r9
0x01c:  add r10, r10, #1
0x020:  b   l1
l2:
0x024:  Exit
""",
        "answer": {
            "regs": [144, 89, 0, 0, 0, 0, 0, 0, 0, 55, 10, 0, 0, 3000, 0x0, 0x28],
            "instr_num": 64,
            "latency": 123
        }
    },
    {
        "name": "Extra_2",
        "content": """main:
0x000:  mov r1, #12
0x004:  bl  fib
0x008:  b   l2
fib:
0x00c:  cmp r1, #3
        bge l1
0x010:  mov r0, #1
0x014:  mov pc, lr
l1:
0x018:  mov r0, #0
0x01c:  sub sp, sp, #12
0x020:  str r1, [sp, #0]
0x024:  str r0, [sp, #4]
0x028:  str lr, [sp, #8]
0x02c:  sub r1, r1, #1
0x030:  bl  fib
0x034:  ldr r9, [sp, #4]
0x038:  ldr r1, [sp, #0]
0x03c:  add r0, r0, r9
0x040:  str r0, [sp, #4]
0x044:  sub r1, r1, #2
0x048:  bl  fib
0x04c:  ldr lr, [sp, #8]
0x050:  ldr r9, [sp, #4]
0x054:  add r0, r0, r9
0x058:  add sp, sp, #12
0x05c:  mov pc, lr
l2:
0x060:  Exit
""",
        "answer": {
            "regs": [144, 2, 0, 0, 0, 0, 0, 0, 0, 89, 0, 0, 0, 3000, 0x8, 0x64],
            "instr_num": 3152,
            "latency": 27765
        }
    },
]

def write_instruction(content):
    f = open("instruction.txt", 'w')
    f.write(content)
    f.close()

def recover_instruction():
    f = open("instruction.txt", 'w')
    f.write(ori_instruction)
    f.close()

def write_latency():
    f = open("latency.txt", 'w')
    f.write(ori_latency)
    f.close()

def compare_result(answer, res):
    correct = 1
    f = open(res, 'r')
    last_lines = f.readlines()[-20:]
    for l in last_lines:
        if "reg[" in l:
            ll = l.split()
            i = 0
            while i < len(ll):
                if "reg[" in ll[i]:
                    reg_num = int(ll[i][4: -1])
                    ans = answer["regs"][reg_num]
                    if ll[i + 2].startswith("0x"):
                        found = int(ll[i + 2], 16)
                    else:
                        found = int(ll[i + 2])
                    if ans != found:
                        print("Reg[%d] error. Correct: %d. Found: %d." % (reg_num, ans, found))
                        correct = 0
                    i = i + 3
                else:
                    i = i + 1
        elif "Instr N" in l:
            ll = l.split()
            ans = answer["instr_num"]
            found = int(ll[2])
            if ans != found:
                print("Instruction number error. Correct: %d. Found: %d." % (ans, found))
                correct = 0
        elif "Latency" in l:
            ll = l.split()
            ans = answer["latency"]
            found = int(ll[1])
            if ans != found:
                print("Latency error. Correct: %d. Found: %d." % (ans, found))
                correct = 0
    if correct == 1:
        print("Clear.")
    return correct

def test(num, exe, res):
    t = test_case[num]
    print("=== %s ===" % t["name"])
    write_instruction(t["content"])
    os.system(exe)
    correct = compare_result(t["answer"], res)
    print("")
    return correct

if __name__ == "__main__":
    write_latency()

    total_1 = 5
    correct_1 = 0
    for i in range(0, total_1):
        correct_1 += test(i, "./pipeline_sim", "./result.txt")

    total_2 = 2
    correct_2 = 0
    for i in range(5, total_1 + total_2):
        correct_2 += test(i, "./pipeline_sim", "./result.txt")

    recover_instruction()

    print("=== Phase 1 ===")
    print("Total: %d" % total_1)
    print("Pass : %d" % correct_1)
    print("=== Phase 2 ===")
    print("Total: %d" % total_2)
    print("Pass : %d" % correct_2)
