#ifndef PIPELINE_SIM_H
#define PIPELINE_SIM_H

#include <iostream>
#include <bits/stdc++.h>
#include <string>

#include "arm_instr.h"

using namespace std;

struct IF_ID {
    Instruction recent_instr;
    int prog_cnt = -1;
};

struct ID_EX {
    int src;
    int r1;
    int r2;
    int address;
    int offset;
    int prog_cnt;
    // Instruction
    ARM_OPC opcode = OPC_INVALID;
    ARM_INSTR_TYPE type = INSTR_TYPE_INVALID;
};

struct EX_MEM {
    int val_arith;
    int val_address;
    int src;
    bool zero;
    int prog_cnt;
    // Instruction
    ARM_OPC opcode = OPC_INVALID;
    ARM_INSTR_TYPE type = INSTR_TYPE_INVALID;
};

struct MEM_WB {
    int write_data;
    int src;
    int prog_cnt;
    // Instruction
    ARM_OPC opcode = OPC_INVALID;
    ARM_INSTR_TYPE type = INSTR_TYPE_INVALID;
};

#endif // PIPELINE_SIM_H
