#ifndef ARM_INSTR_H
#define ARM_INSTR_H

#include <string.h>

using namespace std;

typedef enum {
    REG_INVALID = -1,
    REG_R0, REG_R1, REG_R2, REG_R3,
    REG_R4, REG_R5, REG_R6, REG_R7,
    REG_R8, REG_R9, REG_R10, REG_R11,
    REG_R12, REG_SP, REG_LR, REG_PC
} ARM_REG;

typedef enum {
    OPC_INVALID = 0,
    OPC_ADD, OPC_SUB, OPC_MUL, OPC_MOV,
    OPC_LDR, OPC_STR,
    OPC_CMPBNE, OPC_CMPBGE,
    OPC_BL, OPC_B,
    OPC_EXIT, END_OF_FILE, BUBBLE
} ARM_OPC_TYPE;

static string ARM_OPC_NAME[14] = {
        "",
        "add", "sub", "mul", "mov",
        "ldr", "str",
        "cmp & bne", "cmp & bge",
        "bl", "b",
        "exit", "end of file", "bubble"
};

struct symbol {
    string name;
    int pos;
    int type = 0;
};

struct Instruction {
    ARM_OPC_TYPE opcode = OPC_INVALID;
    int source = -1;
    int operand1 = -1;
    int operand2 = -1;
    int type = -1;
};

#endif // ARM_INSTR_H
