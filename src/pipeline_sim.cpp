#include "pipeline_sim.h"
#include "parser.h"

/**
 * For simplicity, we only use the 16 registers in ARM 32.
 * Memory is 4000 bytes, and top 1000 bytes are preserved.
 * Stack pointer (reg[ARM_REG_SP]) is initialized to 3000.
*/
int reg[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3000, 0, 0};
int mem[4000 / sizeof(int)];

vector<Instruction> instructions;
vector<int> latencies(12, 0);

int PC_src = 0;
int shut_down = 0;
int file_end = 0;

ofstream fout;


// ====================== PIPELINE STAGES ======================

IF_ID Register_IF_ID;
ID_EX Register_ID_EX;
EX_MEM Register_EX_MEM;
MEM_WB Register_MEM_WB;

void flush() {
    reg[ARM_REG_PC] = Register_EX_MEM.val_address * 4;
    Register_IF_ID.recent_instr.opcode = OPC_INVALID;
    Register_IF_ID.recent_instr.type = INSTR_TYPE_INVALID;
    Register_ID_EX.opcode = OPC_INVALID;
    Register_ID_EX.type = INSTR_TYPE_INVALID;
    Register_EX_MEM.opcode = OPC_INVALID;
    Register_EX_MEM.type = INSTR_TYPE_INVALID;
}

/**
 * Fetch
*/
void IF() {
    if (PC_src == 0) {
        if ((reg[ARM_REG_PC] / 4) >= instructions.size()) {
            Instruction end_file;
            end_file.opcode = END_OF_FILE;
            Register_IF_ID.recent_instr = end_file;
            Register_IF_ID.prog_cnt = reg[ARM_REG_PC] / 4;
        } else {
            Register_IF_ID.recent_instr = instructions[reg[ARM_REG_PC] / 4];
            Register_IF_ID.prog_cnt = reg[ARM_REG_PC] / 4;
            fout << ARM_OPC_NAME[instructions[reg[ARM_REG_PC] / 4].opcode] << endl;
            reg[ARM_REG_PC] += 4;
        }
    } else {
        flush();
        PC_src = 0;
    }
}

/**
 * Decode
*/
void ID() {
    /* Stall one cycle for load-use data hazard */
    if ((Register_IF_ID.recent_instr.operand1 == Register_ID_EX.dest
         || Register_IF_ID.recent_instr.operand2 == Register_ID_EX.dest)
        && Register_ID_EX.opcode == OPC_LDR) {
        Register_ID_EX.opcode = BUBBLE;
        Register_ID_EX.dest = -1;
        reg[ARM_REG_PC] -= 4;
        fout << ARM_OPC_NAME[Register_ID_EX.opcode] << endl;
        return;
    }

    Register_ID_EX.prog_cnt = Register_IF_ID.prog_cnt;

    /* Using EX/MEM forward to deal with data hazard */
    if (Register_IF_ID.recent_instr.opcode > OPC_INVALID && Register_IF_ID.recent_instr.opcode <= OPC_MUL) {
        Register_ID_EX.dest = Register_IF_ID.recent_instr.dest;
        bool hazard_r1 = false;
        bool hazard_r2 = false;

        if (Register_IF_ID.recent_instr.operand1 == Register_MEM_WB.dest
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard_r1 = true;
            Register_ID_EX.r1 = Register_MEM_WB.val_data;
        }
        if (Register_IF_ID.recent_instr.operand2 == Register_MEM_WB.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard_r2 = true;
            Register_ID_EX.r2 = Register_MEM_WB.val_data;
        }

        if (Register_IF_ID.recent_instr.operand1 == Register_EX_MEM.dest
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard_r1 = true;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }
        if (Register_IF_ID.recent_instr.operand2 == Register_EX_MEM.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard_r2 = true;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }

        if (!hazard_r1) {
            Register_ID_EX.r1 = reg[Register_IF_ID.recent_instr.operand1];
        }
        if (!hazard_r2) {
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_REG) {
                Register_ID_EX.r2 = reg[Register_IF_ID.recent_instr.operand2];
            }
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_IMM) {
                Register_ID_EX.r2 = Register_IF_ID.recent_instr.operand2;
            }
        }
    }

    if (Register_IF_ID.recent_instr.opcode == OPC_MOV) {
        Register_ID_EX.dest = Register_IF_ID.recent_instr.dest;
        bool hazard = false;

        if (Register_IF_ID.recent_instr.operand1 == Register_MEM_WB.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard = true;
            Register_ID_EX.r1 = Register_MEM_WB.val_data;
            Register_ID_EX.r2 = reg[Register_IF_ID.recent_instr.operand2];
        }
        if (Register_IF_ID.recent_instr.operand1 == Register_EX_MEM.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_LDR) {
            hazard = true;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
            Register_ID_EX.r2 = reg[Register_IF_ID.recent_instr.operand2];
        }

        if (!hazard) {
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_REG) {
                Register_ID_EX.r1 = reg[Register_IF_ID.recent_instr.operand1];
            }
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_IMM) {
                Register_ID_EX.r1 = Register_IF_ID.recent_instr.operand1;
            }
        }

        if (Register_IF_ID.recent_instr.type == INSTR_TYPE_EXTRA) {
            // mov pc, lr
            if (Register_MEM_WB.dest == ARM_REG_LR) {
                Register_ID_EX.r1 = Register_MEM_WB.val_data;
            } else if (Register_EX_MEM.dest == ARM_REG_LR) {
                Register_ID_EX.r1 = Register_EX_MEM.val_arith;
            } else {
                Register_ID_EX.r1 = reg[ARM_REG_LR] / 4;
            }
        }
    }

    if (Register_IF_ID.recent_instr.opcode == OPC_LDR) {
        Register_ID_EX.dest = Register_IF_ID.recent_instr.dest;
        bool hazard = false;

        if (Register_IF_ID.recent_instr.operand1 == Register_MEM_WB.dest
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard = true;
            Register_ID_EX.address = Register_MEM_WB.val_data;
        }
        if (Register_IF_ID.recent_instr.operand1 == Register_EX_MEM.dest
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard = true;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if (!hazard) {
            Register_ID_EX.address = reg[Register_IF_ID.recent_instr.operand1];
        }

        if (Register_IF_ID.recent_instr.type == INSTR_TYPE_IMM) {
            Register_ID_EX.offset = Register_IF_ID.recent_instr.operand2;
        }
        if (Register_IF_ID.recent_instr.type == INSTR_TYPE_EXTRA) {
            Register_ID_EX.address = Register_IF_ID.recent_instr.operand1;
        }
    }

    if (Register_IF_ID.recent_instr.opcode == OPC_STR) {
        Register_ID_EX.dest = reg[Register_IF_ID.recent_instr.dest];
        bool hazard = false;

        if (Register_IF_ID.recent_instr.dest == Register_MEM_WB.dest
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard = true;
            Register_ID_EX.dest = Register_MEM_WB.val_data;
        }
        if (Register_IF_ID.recent_instr.dest == Register_EX_MEM.dest
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard = true;
            Register_ID_EX.dest = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instr.operand1 == Register_MEM_WB.dest
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard = true;
            Register_ID_EX.address = Register_MEM_WB.val_data;
        }
        if (Register_IF_ID.recent_instr.operand1 == Register_EX_MEM.dest
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard = true;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if (!hazard) {
            Register_ID_EX.address = reg[Register_IF_ID.recent_instr.operand1];
        }

        if (Register_IF_ID.recent_instr.type == INSTR_TYPE_IMM) {
            Register_ID_EX.offset = Register_IF_ID.recent_instr.operand2;
        }
    }

    if (Register_IF_ID.recent_instr.opcode >= OPC_CMPBNE && Register_IF_ID.recent_instr.opcode <= OPC_CMPBGE) {
        bool hazard_r1 = false;
        bool hazard_r2 = false;

        if (Register_IF_ID.recent_instr.operand1 == Register_MEM_WB.dest
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard_r1 = true;
            Register_ID_EX.r1 = Register_MEM_WB.val_data;
        }
        if (Register_IF_ID.recent_instr.operand2 == Register_MEM_WB.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
            hazard_r2 = true;
            Register_ID_EX.r2 = Register_MEM_WB.val_data;
        }

        if (Register_IF_ID.recent_instr.operand1 == Register_EX_MEM.dest
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard_r1 = true;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }
        if (Register_IF_ID.recent_instr.operand2 == Register_EX_MEM.dest
            && Register_IF_ID.recent_instr.type == INSTR_TYPE_REG
            && Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
            hazard_r2 = true;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }

        if (!hazard_r1) {
            Register_ID_EX.r1 = reg[Register_IF_ID.recent_instr.operand1];
        }
        if (!hazard_r2) {
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_REG) {
                Register_ID_EX.r2 = reg[Register_IF_ID.recent_instr.operand2];
            }
            if (Register_IF_ID.recent_instr.type == INSTR_TYPE_IMM) {
                Register_ID_EX.r2 = Register_IF_ID.recent_instr.operand2;
            }
        }

        Register_ID_EX.address = Register_IF_ID.recent_instr.dest;
    }

    if (Register_IF_ID.recent_instr.opcode >= OPC_BL && Register_IF_ID.recent_instr.opcode <= OPC_B) {
        Register_ID_EX.address = Register_IF_ID.recent_instr.dest;
    }

    Register_ID_EX.opcode = Register_IF_ID.recent_instr.opcode;
    Register_ID_EX.type = Register_IF_ID.recent_instr.type;
    fout << ARM_OPC_NAME[Register_ID_EX.opcode] << endl;
}

/**
 * Execute
*/
void EX() {
    Register_EX_MEM.prog_cnt = Register_ID_EX.prog_cnt;

    if (Register_ID_EX.opcode > OPC_INVALID && Register_ID_EX.opcode <= OPC_CMPBGE) {
        Register_EX_MEM.dest = Register_ID_EX.dest;

        if (Register_ID_EX.opcode >= OPC_ADD && Register_ID_EX.opcode <= OPC_MOV) {
            switch (Register_ID_EX.opcode) {
                case OPC_ADD:
                    Register_EX_MEM.val_arith = Register_ID_EX.r1 + Register_ID_EX.r2;
                    break;
                case OPC_SUB:
                    Register_EX_MEM.val_arith = Register_ID_EX.r1 - Register_ID_EX.r2;
                    break;
                case OPC_MUL:
                    Register_EX_MEM.val_arith = Register_ID_EX.r1 * Register_ID_EX.r2;
                    break;
                case OPC_MOV:
                    Register_EX_MEM.val_arith = Register_ID_EX.r1;
                    break;
                default:
                    break;
            }
        }
        if (Register_ID_EX.opcode >= OPC_LDR && Register_ID_EX.opcode <= OPC_STR) {
            if (Register_ID_EX.type == INSTR_TYPE_IMM) {
                Register_EX_MEM.val_address = Register_ID_EX.address + Register_ID_EX.offset;
            } else {
                Register_EX_MEM.val_address = Register_ID_EX.address;
            }
        }
        if (Register_ID_EX.opcode >= OPC_CMPBNE && Register_ID_EX.opcode <= OPC_CMPBGE) {
            if (Register_ID_EX.opcode == OPC_CMPBNE) {
                Register_EX_MEM.zero = (Register_ID_EX.r1 == Register_ID_EX.r2);
            }
            if (Register_ID_EX.opcode == OPC_CMPBGE) {
                Register_EX_MEM.zero = Register_ID_EX.r1 < Register_ID_EX.r2;
            }
            Register_EX_MEM.val_address = Register_ID_EX.address;
        }
    }

    if (Register_ID_EX.opcode >= OPC_BL && Register_ID_EX.opcode <= OPC_B) {
        Register_EX_MEM.val_address = Register_ID_EX.address;
        Register_EX_MEM.zero = true;
    }

    Register_EX_MEM.opcode = Register_ID_EX.opcode;
    Register_EX_MEM.type = Register_ID_EX.type;
    fout << ARM_OPC_NAME[Register_EX_MEM.opcode] << endl;
}

/**
 * Memory
*/
void MEM() {
    Register_MEM_WB.prog_cnt = Register_EX_MEM.prog_cnt;

    if (Register_EX_MEM.opcode > OPC_INVALID && Register_EX_MEM.opcode <= OPC_MOV) {
        Register_MEM_WB.val_data = Register_EX_MEM.val_arith;
        Register_MEM_WB.dest = Register_EX_MEM.dest;
    }

    if (Register_EX_MEM.opcode == OPC_LDR) {
        if (Register_EX_MEM.type != INSTR_TYPE_EXTRA) {
            Register_MEM_WB.val_data = mem[(Register_EX_MEM.val_address) / 4];
            fout << "mem[" << Register_EX_MEM.val_address << "] = "
                 << mem[(Register_EX_MEM.val_address) / 4] << endl;
            Register_MEM_WB.dest = Register_EX_MEM.dest;
        } else {
            Register_MEM_WB.val_data = (Register_EX_MEM.val_address);
            Register_MEM_WB.dest = Register_EX_MEM.dest;
        }
    }

    if (Register_EX_MEM.opcode == OPC_STR) {
        mem[(Register_EX_MEM.val_address) / 4] = Register_EX_MEM.dest;
        fout << "mem[" << Register_EX_MEM.val_address << "] = "
             << Register_EX_MEM.dest << endl;
    }

    if (Register_EX_MEM.opcode == OPC_MOV && Register_EX_MEM.type == INSTR_TYPE_EXTRA) {
        Register_EX_MEM.val_address = Register_EX_MEM.val_arith;
        PC_src = 1;
    }

    if (Register_EX_MEM.opcode >= OPC_CMPBNE && Register_EX_MEM.opcode <= OPC_CMPBGE && Register_EX_MEM.zero == 0) {
        PC_src = 1;
    }

    if (Register_EX_MEM.opcode == OPC_BL) {
        reg[ARM_REG_LR] = (Register_EX_MEM.prog_cnt + 1) * 4;
        PC_src = 1;
    }

    if (Register_EX_MEM.opcode == OPC_B) {
        PC_src = 1;
    }

    Register_MEM_WB.opcode = Register_EX_MEM.opcode;
    Register_MEM_WB.type = Register_EX_MEM.type;
    fout << ARM_OPC_NAME[Register_EX_MEM.opcode] << endl;
}

/**
 * Writeback
*/
void WB() {
    if (Register_MEM_WB.opcode > OPC_INVALID && Register_MEM_WB.opcode <= OPC_LDR) {
        reg[Register_MEM_WB.dest] = Register_MEM_WB.val_data;
    } else if (Register_MEM_WB.opcode == OPC_EXIT) {
        shut_down = 1;
    } else if (Register_MEM_WB.opcode == END_OF_FILE) {
        file_end = 1;
    }
    fout << ARM_OPC_NAME[Register_MEM_WB.opcode] << endl;
}

// ====================== PIPELINE STAGES ======================


void print_register() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int num = 4 * i + j;
            char reg_str[10], eq_str[10];
            sprintf(reg_str, "reg[%d]", num);
            if (num < ARM_REG_LR) {
                sprintf(eq_str, "= %d", reg[num]);
            } else {
                sprintf(eq_str, "= 0x%x", reg[num]);
            }
            fout << left << setw(10) << reg_str << setw(12) << eq_str << "\t";
        }
        fout << endl;
    }
    fout << endl;
}

int compute_latency() {
    vector<int> latency_pipe(5, 0);
    // IF
    latency_pipe[0] = 1;
    // ID
    latency_pipe[1] = 1;
    // EX
    if (Register_ID_EX.opcode > OPC_INVALID && Register_ID_EX.opcode <= OPC_MOV) {
        latency_pipe[2] = latencies[Register_ID_EX.opcode - 1];
    } else if (Register_ID_EX.opcode == OPC_LDR && Register_ID_EX.type == INSTR_TYPE_EXTRA) {
        latency_pipe[2] = latencies[5];
    } else if (Register_ID_EX.opcode >= OPC_CMPBNE && Register_ID_EX.opcode <= OPC_CMPBGE) {
        latency_pipe[2] = latencies[7];
    } else {
        latency_pipe[2] = 1;
    }
    // MEM
    if (Register_EX_MEM.opcode == OPC_LDR) {
        latency_pipe[3] = latencies[4];
    } else if (Register_EX_MEM.opcode == OPC_STR) {
        latency_pipe[3] = latencies[6];
    } else if (Register_EX_MEM.opcode >= OPC_CMPBNE && Register_EX_MEM.opcode <= OPC_B) {
        latency_pipe[3] = latencies[Register_EX_MEM.opcode + 1];
    } else {
        latency_pipe[3] = 1;
    }
    // WB
    latency_pipe[4] = 1;
    return *max_element(latency_pipe.begin(), latency_pipe.end());
}

int main() {

    if (parse_file(latencies, instructions)) {
        return -1;
    }

    int instr_num = 0, latency = 0;

    fout.open("result.txt");
    print_register();

    while (!shut_down && !file_end) {
        if (Register_MEM_WB.opcode >= OPC_ADD && Register_MEM_WB.opcode <= OPC_B) {
            instr_num++;
        }

        fout << "======= WB =======" << endl;
        WB();

        fout << "======= MEM ======" << endl;
        MEM();

        if (PC_src == 0) {
            fout << "======= EX =======" << endl;
            EX();

            fout << "======= ID =======" << endl;
            ID();
        } else {
            fout << "======= EX =======" << endl;
            fout << "flush" << endl;

            fout << "======= ID =======" << endl;
            fout << "flush" << endl;
        }

        fout << "======= IF =======" << endl;
        IF();

        fout << endl;
        print_register();

        latency += compute_latency();

        fout << "Instr N: " << instr_num << endl;
        fout << "Latency: " << latency << endl << endl << endl;
    }

    fout << "CPI: " << float(latency) / float(instr_num) << endl;
    fout.close();

}
