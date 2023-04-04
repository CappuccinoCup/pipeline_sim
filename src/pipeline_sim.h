#ifndef PIPELINE_SIM_H
#define PIPELINE_SIM_H

#include <iostream>
#include <bits/stdc++.h>
#include <string.h>
#include <ctype.h>

using namespace std;

struct symbol {
    string name;
    int pos;
    int type = 0;
};

typedef enum {
    INVALID = 0,
    ADD,
    SUB,
    MUL,
    MOV,
    LDR,
    STR,
    CMPBNE,
    CMPBGE,
    BL,
    B,
    EXIT,
    END_OF_FILE,
    BUBBLE
} ARM_OPC_TYPE;

struct Instruction {
    ARM_OPC_TYPE opcode = INVALID;
    int source = -1;
    int operand1 = -1;
    int operand2 = -1;
    int type = -1;
};

void print_opcode(ARM_OPC_TYPE opcode) {
    if (opcode == ADD) cout << "add" << endl;
    else if (opcode == SUB) cout << "sub" << endl;
    else if (opcode == MUL) cout << "mul" << endl;
    else if (opcode == MOV) cout << "mov" << endl;
    else if (opcode == LDR) cout << "ldr" << endl;
    else if (opcode == STR) cout << "str" << endl;
    else if (opcode == CMPBNE) cout << "cmp & bne" << endl;
    else if (opcode == CMPBGE) cout << "cmp & bge" << endl;
    else if (opcode == BL) cout << "bl" << endl;
    else if (opcode == B) cout << "b" << endl;
    else if (opcode == EXIT) cout << "exit" << endl;
    else if (opcode == END_OF_FILE) cout << "end of file" << endl;
}

struct IF_ID {
    Instruction recent_instruction;
    int prog_cnt = -1;
};

struct ID_EX {
    int src;
    int r1;
    int r2;
    int address;
    int offset;
    int prog_cnt;
    ARM_OPC_TYPE opcode = INVALID;
    int type;
};

struct EX_MEM {
    int val_arith;
    int val_address;
    int src;
    bool zero;
    int prog_cnt;
    ARM_OPC_TYPE opcode = INVALID;
    int type;
};

struct MEM_WB {
    int write_data;
    int src;
    int prog_cnt;
    ARM_OPC_TYPE opcode = INVALID;
    int type;
};

#endif // PIPELINE_SIM_H
