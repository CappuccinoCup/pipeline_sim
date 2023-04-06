#ifndef ARM_INSTR_H
#define ARM_INSTR_H

#include <string>

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
} ARM_OPC;

static const string ARM_OPC_NAME[14] = {
        "",
        "add", "sub", "mul", "mov",
        "ldr", "str",
        "cmp & bne", "cmp & bge",
        "bl", "b",
        "exit", "", "bubble"
};

typedef enum {
    OPC_TYPE_INVALID = 0,
    OPC_TYPE_ADD_REG, OPC_TYPE_ADD_IMM, OPC_TYPE_ADD_SP_IMM,
    OPC_TYPE_SUB_REG, OPC_TYPE_SUB_IMM, OPC_TYPE_SUB_SP_IMM,
    OPC_TYPE_MUL_REG, OPC_TYPE_MUL_IMM,
    OPC_TYPE_MOV_REG, OPC_TYPE_MOV_IMM, OPC_TYPE_MOV_PC_LR,
    OPC_TYPE_LDR_LABEL, OPC_TYPE_LDR_REG, OPC_TYPE_LDR_REG_OFFSET,
    OPC_TYPE_LDR_SP_OFFSET, OPC_TYPE_LDR_LR_SP_OFFSET,
    OPC_TYPE_STR_REG, OPC_TYPE_STR_REG_OFFSET,
    OPC_TYPE_STR_SP_OFFSET, OPC_TYPE_STR_LR_SP_OFFSET,
    OPC_TYPE_CMP_BNE_REG, OPC_TYPE_CMP_BNE_IMM,
    OPC_TYPE_CMP_BGE_REG, OPC_TYPE_CMP_BGE_IMM,
} ARM_OPC_TYPE;

typedef enum {
    INSTR_TYPE_INVALID = 0,
    INSTR_TYPE_REG,
    INSTR_TYPE_IMM,
    INSTR_TYPE_EXTRA
} ARM_INSTR_TYPE;

struct Instruction {
    ARM_OPC opcode = OPC_INVALID;
    ARM_INSTR_TYPE type = INSTR_TYPE_INVALID;
    int dest = -1;
    int operand1 = -1;
    int operand2 = -1;
};

typedef enum {
    SYMBOL_INVALID = -1,
    SYMBOL_JMP_LABEL,
    SYMBOL_LDR_LABEL,
    SYMBOL_DATA_LABEL
} SYMBOL_TYPE;

struct symbol {
    SYMBOL_TYPE type = SYMBOL_INVALID;
    string name;
    int pos = -1;
};

#endif // ARM_INSTR_H
