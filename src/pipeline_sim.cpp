#include "pipeline_sim.h"
#include "parser.h"
#include "color.h"

ifstream fin;
vector <string> lexeme;

int prog = 0;
int r[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1000, 0, 0};
int mem[6000];
int freemem = 5000;

vector <symbol> symbol_table;
vector <symbol> symbol_temps;

bool branch_predictor_bit[2] = {0, 0};

int shut_down = 0;
int end_file_inst = 0;

vector <Instruction> instruction_file;

//###########################INSTRUCTION FILE######################################
int make_instruction_file() {
    int error = 0;
    while (prog < lexeme.size()) {
        //cout<<prog<<endl;
        if (lexeme[prog] == "Add" || lexeme[prog] == "add" || lexeme[prog] == "ADD" || lexeme[prog] == "Sub" ||
            lexeme[prog] == "sub" || lexeme[prog] == "SUB" || lexeme[prog] == "Mul" || lexeme[prog] == "mul" ||
            lexeme[prog] == "MUL" || lexeme[prog] == "Mov" || lexeme[prog] == "mov" || lexeme[prog] == "MOV" ||
            lexeme[prog] == "Ldr" || lexeme[prog] == "ldr" || lexeme[prog] == "LDR" || lexeme[prog] == "Str" ||
            lexeme[prog] == "str" || lexeme[prog] == "STR") {

            int p1 = prog;
            //int p=1;//
            int p = execute(prog);
            //int q=compare(prog);
            prog = p1;
            if (p == -1) {
                cout << "Error in " << lexeme[prog] << " instruction" << endl;
                prog++;
                error++;
            } else {

                if (lexeme[prog] == "Add" || lexeme[prog] == "add" || lexeme[prog] == "ADD")       //Addition
                {
                    Instruction temp_i;
                    temp_i.opcode = ADD;

                    //Add rd,rop1,rop2
                    if (lexeme[prog + 5][0] == 'r') {
                        temp_i.source = stoi(lexeme[prog + 1].substr(1));
                        temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                        temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                        temp_i.type = 1;
                    } else {

                        //Add rd ,rop1,#Imm
                        if (lexeme[prog + 3][0] == 'r') {
                            temp_i.source = stoi(lexeme[prog + 1].substr(1));
                            temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if (lexeme[prog + 3] == "SP" || lexeme[prog + 3] == "Sp" || lexeme[prog + 3] == "sp") {
                            temp_i.source = 13;
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instruction_file.push_back(temp_i);
                    prog += 6;
                } else if (lexeme[prog] == "Sub" || lexeme[prog] == "sub" || lexeme[prog] == "SUB")      //Subtract
                {
                    Instruction temp_i;
                    temp_i.opcode = SUB;

                    //Sub rd,rop1,rop2
                    if (lexeme[prog + 5][0] == 'r') {
                        temp_i.source = stoi(lexeme[prog + 1].substr(1));
                        temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                        temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                        temp_i.type = 1;
                    } else {

                        //Add rd ,rop1,#Imm
                        if (lexeme[prog + 3][0] == 'r') {
                            temp_i.source = stoi(lexeme[prog + 1].substr(1));
                            temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if (lexeme[prog + 3] == "SP" || lexeme[prog + 3] == "Sp" || lexeme[prog + 3] == "sp") {
                            temp_i.source = 13;
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instruction_file.push_back(temp_i);
                    prog += 6;
                } else if (lexeme[prog] == "Mul" || lexeme[prog] == "mul" || lexeme[prog] == "MUL")      //Multiply
                {
                    Instruction temp_i;
                    temp_i.opcode = MUL;
                    temp_i.source = stoi(lexeme[prog + 1].substr(1));
                    temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                    //Mul rd,rop1,rop2
                    if (lexeme[prog + 5][0] == 'r') {
                        temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                        temp_i.type = 1;
                    }
                        //Mul rd, rop1,#Imm
                    else {
                        temp_i.operand2 = stoi(lexeme[prog + 5].substr(1));
                        temp_i.type = 2;
                    }
                    instruction_file.push_back(temp_i);
                    prog += 6;
                } else if (lexeme[prog] == "Mov" || lexeme[prog] == "mov" || lexeme[prog] == "MOV")      //Move
                {
                    Instruction temp_i;

                    temp_i.opcode = MOV;
                    if ((lexeme[prog + 1] == "pc" || lexeme[prog + 1] == "Pc" || lexeme[prog + 1] == "PC") &&
                        (lexeme[prog + 3] == "lr" || lexeme[prog + 3] == "Lr" || lexeme[prog + 3] == "LR")) {
                        temp_i.source = -1;
                        temp_i.operand1 = 14;
                        temp_i.operand1 = -1;
                        temp_i.type = 3;
                        instruction_file.push_back(temp_i);
                    } else {
                        temp_i.source = stoi(lexeme[prog + 1].substr(1));
                        temp_i.operand2 = -1;
                        if (lexeme[prog + 3][0] == 'r')  //Mov rd rop1
                        {
                            temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                            temp_i.type = 1;
                        } else//Mov rd #12
                        {
                            temp_i.operand1 = stoi(lexeme[prog + 3].substr(1));
                            temp_i.type = 2;
                        }
                        instruction_file.push_back(temp_i);
                    }
                    prog += 4;

                } else if (lexeme[prog] == "Ldr" || lexeme[prog] == "ldr" || lexeme[prog] == "LDR")          //Load
                {
                    Instruction temp_i;
                    temp_i.opcode = LDR;
                    if (lexeme[prog + 1] == "LR" || lexeme[prog + 1] == "Lr" || lexeme[prog + 1] == "lr") {
                        temp_i.source = 14;
                    } else {
                        temp_i.source = stoi(lexeme[prog + 1].substr(1));
                    }

                    if (lexeme[prog + 3][0] == '[')      //Ldr rd, [rop1]
                    {
                        if (lexeme[prog + 5][0] == ']') {
                            //ldr rd,[sp]
                            if (lexeme[prog + 4] == "SP" || lexeme[prog + 4] == "Sp" || lexeme[prog + 4] == "sp") {
                                temp_i.operand1 = 13;
                                temp_i.operand2 = -1;
                                temp_i.type = 1;
                                prog += 6;
                            } else {
                                temp_i.operand1 = stoi(lexeme[prog + 4].substr(1));
                                temp_i.operand2 = -1;
                                temp_i.type = 1;
                                prog += 6;
                            }
                        } else {
                            //ldr rd,[sp,#IMM]
                            if (lexeme[prog + 4] == "SP" || lexeme[prog + 4] == "Sp" || lexeme[prog + 4] == "sp") {
                                temp_i.operand1 = 13;
                                temp_i.operand2 = stoi(lexeme[prog + 6].substr(1));
                                temp_i.type = 2;
                                prog += 8;
                            } else {
                                temp_i.operand1 = stoi(lexeme[prog + 4].substr(1));
                                temp_i.operand2 = stoi(lexeme[prog + 6].substr(1));
                                temp_i.type = 2;
                                prog += 8;
                            }
                        }
                    } else if (lexeme[prog + 3][0] == '=')      //Ldr rd, =AA
                    {
                        temp_i.operand1 = -2;
                        symbol tem;                                       //change
                        tem.name = lexeme[prog + 3].substr(1);
                        tem.pos = instruction_file.size();
                        tem.type = 1;
                        symbol_temps.push_back(tem);
                        temp_i.operand2 = -1;
                        temp_i.type = 3;
                        prog += 4;                                                    //change
                    }
                    instruction_file.push_back(temp_i);
                } else        //Store
                {
                    Instruction temp_i;
                    temp_i.opcode = STR;

                    if (lexeme[prog + 1] == "LR" || lexeme[prog + 1] == "Lr" || lexeme[prog + 1] == "lr") {
                        temp_i.source = 14;
                    } else {
                        temp_i.source = stoi(lexeme[prog + 1].substr(1));
                    }
                    if (lexeme[prog + 4][0] == 'r' && lexeme[prog + 5][0] == ']')      //Str rd, [rop1]
                    {
                        if (lexeme[prog + 4] == "SP" || lexeme[prog + 4] == "Sp" || lexeme[prog + 4] == "sp") {
                            temp_i.operand1 = 13;
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            prog += 6;
                        } else {
                            temp_i.operand1 = stoi(lexeme[prog + 4].substr(1));
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            prog += 6;
                        }
                    } else        //Str rd, [rop1, #21]
                    {
                        if (lexeme[prog + 4] == "SP" || lexeme[prog + 4] == "Sp" || lexeme[prog + 4] == "sp") {
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog + 6].substr(1));
                            temp_i.type = 2;
                            prog += 8;
                        } else {
                            temp_i.operand1 = stoi(lexeme[prog + 4].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog + 6].substr(1));
                            temp_i.type = 2;
                            prog += 8;
                        }
                    }
                    instruction_file.push_back(temp_i);
                }
            }

        } else if (lexeme[prog] == "Cmp" || lexeme[prog] == "cmp" || lexeme[prog] == "CMP")         //compare
        {
            int q1 = prog;
            int q = compare(prog);
            prog = q1;
            if (q == -1) {
                cout << "Error in cmp instruction" << endl;
                prog++;
                error++;
            } else if (lexeme[prog + 4] == "Bne" || lexeme[prog + 4] == "bne" ||
                       lexeme[prog + 4] == "BNE")      //cmp and bne
            {
                Instruction temp_i;
                temp_i.opcode = CMPBNE;
                temp_i.source = -2;                             //change
                symbol tem;                                   //change
                tem.name = lexeme[prog + 5];
                tem.pos = instruction_file.size();
                symbol_temps.push_back(tem);
                if (lexeme[prog + 1][0] == 'r' && lexeme[prog + 3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog + 3].substr(1));                                   //change
                    temp_i.type = 1;
                    prog += 6;
                } else if (lexeme[prog + 1][0] == 'r' && lexeme[prog + 3][0] == '#')        //cmp rd, #5
                {
                    temp_i.operand1 = stoi(lexeme[prog + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog + 3].substr(1));                                   //change
                    temp_i.type = 2;
                    prog += 6;
                }
                instruction_file.push_back(temp_i);
            } else if (lexeme[prog + 4] == "Bge" || lexeme[prog + 4] == "bge" ||
                       lexeme[prog + 4] == "BGE")         //cmp and bge
            {
                Instruction temp_i;
                temp_i.opcode = CMPBGE;
                temp_i.source = -2;                                                             //change
                symbol tem;                                                                       //change
                tem.name = lexeme[prog + 5];
                tem.pos = instruction_file.size();
                symbol_temps.push_back(tem);
                if (lexeme[prog + 1][0] == 'r' && lexeme[prog + 3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog + 3].substr(1));
                    temp_i.type = 1;
                    prog += 6;
                } else if (lexeme[prog + 1][0] == 'r' && lexeme[prog + 3][0] == '#')        //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog + 3].substr(1));
                    temp_i.type = 2;
                    prog += 6;
                }
                instruction_file.push_back(temp_i);
            }

        } else if (lexeme[prog] == "BL" || lexeme[prog] == "bl" || lexeme[prog] == "Bl")     //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = BL;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symbol tem;                                                                   //change
            tem.name = lexeme[prog + 1];
            tem.pos = instruction_file.size();
            symbol_temps.push_back(tem);
            prog += 2;
            instruction_file.push_back(temp_i);
        } else if (lexeme[prog] == "B" || lexeme[prog] == "b")        //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = B;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symbol tem;                                                                   //change
            tem.name = lexeme[prog + 1];
            tem.pos = instruction_file.size();
            symbol_temps.push_back(tem);
            prog += 2;
            instruction_file.push_back(temp_i);
        } else if (lexeme[prog] == ":") {
            symbol tem;
            tem.name = lexeme[prog - 1];
            if (lexeme[prog + 1] == ".space") {
                tem.pos = freemem;
                freemem = freemem + stoi(lexeme[prog + 2]);
                tem.type = 2;
            } else {
                tem.pos = instruction_file.size();
            }
            symbol_table.push_back(tem);
            prog++;
        } else if (lexeme[prog] == "Exit" || lexeme[prog] == "exit" || lexeme[prog] == "EXIT" ||
                   lexeme[prog] == "Swi_Exit" || lexeme[prog] == "swi_exit" || lexeme[prog] == "SWI_EXIT") {
            Instruction temp_i;
            temp_i.opcode = EXIT;
            temp_i.source = -1;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = -1;
            instruction_file.push_back(temp_i);
            //symbol tem;
            prog++;
        } else prog++;
    }
    symbol tem_e;
    tem_e.name = "Exit";
    tem_e.pos = instruction_file.size();
    symbol_table.push_back(tem_e);
    tem_e.name = "exit";
    symbol_table.push_back(tem_e);
    tem_e.name = "EXIT";
    symbol_table.push_back(tem_e);
    tem_e.name = "Swi_Exit";
    symbol_table.push_back(tem_e);
    tem_e.name = "SWI_EXIT";
    symbol_table.push_back(tem_e);
    tem_e.name = "swi_exit";
    symbol_table.push_back(tem_e);

    return error;
}
////############################END INSTRUCTION FILE#################################################


// ====================== PIPELINE COMPONENTS ======================
int PCSrc = 0;
int instruction_total = 0;

IF_ID Register_IF_ID;
ID_EX Register_ID_EX;
EX_MEM Register_EX_MEM;
MEM_WB Register_MEM_WB;
// ====================== PIPELINE COMPONENTS ======================


// ====================== PIPELINE STAGES ======================
void flush() {
    prog = Register_EX_MEM.val_address - 1;
    Register_IF_ID.recent_instruction.opcode = INVALID;
    Register_IF_ID.recent_instruction.type = -1;
    Register_ID_EX.opcode = INVALID;
    Register_ID_EX.type = -1;
    Register_EX_MEM.opcode = INVALID;
    Register_EX_MEM.type = -1;
}

void IF() {
    if (PCSrc == 0) {
        prog++;
        if (prog >= instruction_file.size()) {
            Instruction end_file;
            end_file.opcode = END_OF_FILE;
            end_file.source = -1;
            end_file.operand1 = -1;
            end_file.operand2 = -1;
            end_file.type = -1;
            Register_IF_ID.recent_instruction = end_file;
            Register_IF_ID.prog_cnt = prog;
        } else if ((Register_IF_ID.recent_instruction.source == instruction_file[prog].operand1
                 || Register_IF_ID.recent_instruction.source == instruction_file[prog].operand2) 
                 && Register_IF_ID.recent_instruction.opcode == LDR) {
            cout << "stall" << endl;
            prog--;
            Instruction bubble;
            bubble.opcode = BUBBLE;
            bubble.source = -1;
            bubble.operand1 = -1;
            bubble.operand2 = -1;
            bubble.type = -1;
            Register_IF_ID.recent_instruction = bubble;
            Register_IF_ID.prog_cnt = prog;
        } else {
            Register_IF_ID.recent_instruction = instruction_file[prog];
            Register_IF_ID.prog_cnt = prog;
            print_opcode(instruction_file[prog].opcode);
        }
    } else {
        flush();
        PCSrc = 0;
    }
}

void ID() {
    Register_ID_EX.prog_cnt = Register_IF_ID.prog_cnt;

    if (Register_IF_ID.recent_instruction.opcode > INVALID && Register_IF_ID.recent_instruction.opcode <= MUL) {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard_r1 = 0;
        bool hazard_r2 = 0;

        if (Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= LDR) {
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
            hazard_r1 = 1;
        } else if (Register_IF_ID.recent_instruction.operand2 == Register_MEM_WB.src 
                && Register_IF_ID.recent_instruction.type == 1 
                && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= LDR) {
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= LDR) {
            cout << Register_EX_MEM.val_arith << endl;
            hazard_r1 = 1;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.operand2 == Register_EX_MEM.src 
         && Register_IF_ID.recent_instruction.type == 1 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= LDR) {
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }

        if (hazard_r1 == 0) {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        }

        if (Register_IF_ID.recent_instruction.type == 1 && hazard_r2 == 0) {
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
        } else if (Register_IF_ID.recent_instruction.type == 2) {
            Register_ID_EX.r2 = Register_IF_ID.recent_instruction.operand2;
        }
    }
    
    if (Register_IF_ID.recent_instruction.opcode == MOV) {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard = 0;

        if (Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src 
         && Register_IF_ID.recent_instruction.type == 1 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src 
         && Register_IF_ID.recent_instruction.type == 1 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.type == 1 && hazard == 0) {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        } else if (Register_IF_ID.recent_instruction.type == 2 && hazard == 0) {
            Register_ID_EX.r1 = Register_IF_ID.recent_instruction.operand1;
        } else if (Register_IF_ID.recent_instruction.type == 3) {
            if (Register_MEM_WB.src == 14) {
                Register_ID_EX.r1 = Register_MEM_WB.write_data;
            } else if (Register_EX_MEM.src == 14) {
                Register_ID_EX.r1 = Register_EX_MEM.val_arith;
            } else
                Register_ID_EX.r1 = r[14];
        }
    }
    
    if (Register_IF_ID.recent_instruction.opcode == LDR) {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard = 0;
        bool hazard_s = 0;

        if (Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.address = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.type == 1 && hazard == 0) {
            Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
        } else if (Register_IF_ID.recent_instruction.type == 2) {
            if (hazard == 0) {
                Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
            }
            Register_ID_EX.offset = Register_IF_ID.recent_instruction.operand2;
        } else if (Register_IF_ID.recent_instruction.type == 3) {
            Register_ID_EX.address = Register_IF_ID.recent_instruction.operand1;
        }
    }
    
    if (Register_IF_ID.recent_instruction.opcode == STR) {
        Register_ID_EX.src = r[Register_IF_ID.recent_instruction.source];
        bool hazard = 0;
        bool hazard_s = 0;

        if (Register_IF_ID.recent_instruction.source == Register_MEM_WB.src 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.src = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.source == Register_EX_MEM.src 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.src = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.address = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard = 1;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.type == 1 && hazard == 0) {
            Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
        } else if (Register_IF_ID.recent_instruction.type == 2) {
            if (hazard == 0) {
                Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
            }
            Register_ID_EX.offset = Register_IF_ID.recent_instruction.operand2;
        }
    }
    
    if (Register_IF_ID.recent_instruction.opcode >= CMPBNE && Register_IF_ID.recent_instruction.opcode <= CMPBGE) {
        bool hazard_r1 = 0;
        bool hazard_r2 = 0;

        if (Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src 
         && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
            hazard_r1 = 1;
        } else if (Register_IF_ID.recent_instruction.operand2 == Register_MEM_WB.src 
                && Register_IF_ID.recent_instruction.type == 1 
                && Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= STR) {
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_MEM_WB.write_data;
        }

        if (Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard_r1 = 1;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }

        if (Register_IF_ID.recent_instruction.operand2 == Register_EX_MEM.src 
         && Register_IF_ID.recent_instruction.type == 1 
         && Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= STR) {
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }

        if (hazard_r1 == 0) {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        }

        if (Register_IF_ID.recent_instruction.type == 1 && hazard_r2 == 0) {
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
        } else if (Register_IF_ID.recent_instruction.type == 2) {
            Register_ID_EX.r2 = Register_IF_ID.recent_instruction.operand2;
        }

        Register_ID_EX.address = Register_IF_ID.recent_instruction.source;
    }
    
    if (Register_IF_ID.recent_instruction.opcode >= BL && Register_IF_ID.recent_instruction.opcode <= B) {
        Register_ID_EX.address = Register_IF_ID.recent_instruction.source;
    }

    Register_ID_EX.opcode = Register_IF_ID.recent_instruction.opcode;
    Register_ID_EX.type = Register_IF_ID.recent_instruction.type;
    print_opcode(Register_IF_ID.recent_instruction.opcode);
}

void EX() {
    Register_EX_MEM.prog_cnt = Register_ID_EX.prog_cnt;

    if (Register_ID_EX.opcode > INVALID && Register_ID_EX.opcode <= CMPBGE) {
        Register_EX_MEM.src = Register_ID_EX.src;

        if (Register_ID_EX.opcode >= ADD && Register_ID_EX.opcode <= MOV) {
            switch (Register_ID_EX.opcode) {
            case ADD:
                Register_EX_MEM.val_arith = Register_ID_EX.r1 + Register_ID_EX.r2;
                break;
            case SUB:
                Register_EX_MEM.val_arith = Register_ID_EX.r1 - Register_ID_EX.r2;
                break;
            case MUL:
                Register_EX_MEM.val_arith = Register_ID_EX.r1 * Register_ID_EX.r2;
                break;
            case MOV:
                Register_EX_MEM.val_arith = Register_ID_EX.r1;
                break;
            default:
                break;
            }
        }

        if (Register_ID_EX.opcode >= LDR && Register_ID_EX.opcode <= STR) {
            if (Register_ID_EX.type == 2) {
                Register_EX_MEM.val_address = Register_ID_EX.address + Register_ID_EX.offset;
            } else {
                Register_EX_MEM.val_address = Register_ID_EX.address;
            }
        }

        if (Register_ID_EX.opcode >= CMPBNE && Register_ID_EX.opcode <= CMPBGE) {
            if (Register_ID_EX.opcode == CMPBNE && Register_ID_EX.r1 == Register_ID_EX.r2) {
                Register_EX_MEM.zero = 1;
            } else if (Register_ID_EX.opcode == CMPBGE && Register_ID_EX.r1 < Register_ID_EX.r2) {
                Register_EX_MEM.zero = 1;
            } else {
                Register_EX_MEM.zero = 0;
            }
            Register_EX_MEM.val_address = Register_ID_EX.address;
        }
    }
    
    if (Register_ID_EX.opcode >= BL && Register_ID_EX.opcode <= B) {
        Register_EX_MEM.val_address = Register_ID_EX.address;
        Register_EX_MEM.zero = 1;
    }

    Register_EX_MEM.opcode = Register_ID_EX.opcode;
    Register_EX_MEM.type = Register_ID_EX.type;
    print_opcode(Register_ID_EX.opcode);
}

void MEM() {
    Register_MEM_WB.prog_cnt = Register_EX_MEM.prog_cnt;

    if (Register_EX_MEM.opcode > INVALID && Register_EX_MEM.opcode <= MOV) {
        Register_MEM_WB.write_data = Register_EX_MEM.val_arith;
        Register_MEM_WB.src = Register_EX_MEM.src;
    }
    
    if (Register_EX_MEM.opcode == LDR) {
        if (Register_EX_MEM.type != 3) {
            Register_MEM_WB.write_data = mem[(Register_EX_MEM.val_address) / 4];
            cout << "mem[" << (Register_EX_MEM.val_address) / 4 << "]=" << mem[(Register_EX_MEM.val_address) / 4]
                 << endl;
            Register_MEM_WB.src = Register_EX_MEM.src;
        } else {
            Register_MEM_WB.write_data = (Register_EX_MEM.val_address);
            Register_MEM_WB.src = Register_EX_MEM.src;
        }
    }
    
    if (Register_EX_MEM.opcode == STR) {
        mem[(Register_EX_MEM.val_address) / 4] = Register_EX_MEM.src;
        cout << "mem[" << Register_EX_MEM.val_address / 4 << "] = " << Register_EX_MEM.src 
             << endl;
    }

    if (Register_EX_MEM.opcode == MOV && Register_EX_MEM.type == 3) {
        Register_EX_MEM.val_address = Register_EX_MEM.val_arith;
        PCSrc = 1;
    }

    if (Register_EX_MEM.opcode >= CMPBNE && Register_EX_MEM.opcode <= CMPBGE && Register_EX_MEM.zero == 0) {
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }

    if (Register_EX_MEM.opcode == BL) {
        r[14] = Register_EX_MEM.prog_cnt + 1;
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }

    if (Register_EX_MEM.opcode == B) {
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }

    Register_MEM_WB.opcode = Register_EX_MEM.opcode;
    Register_MEM_WB.type = Register_EX_MEM.type;
    print_opcode(Register_EX_MEM.opcode);
}

void WB() {
    if (Register_MEM_WB.opcode > INVALID && Register_MEM_WB.opcode <= LDR) {
        r[Register_MEM_WB.src] = Register_MEM_WB.write_data;
    } else if (Register_MEM_WB.opcode == EXIT) {
        shut_down = 1;
    } else if (Register_MEM_WB.opcode == END_OF_FILE) {
        end_file_inst = 1;
    }

    print_opcode(Register_MEM_WB.opcode);
}
// ====================== PIPELINE STAGES ======================


// ====================== LATENCY ANALYSIS ======================
string latency_insn[13] = {"add", "sub", "mul", "mov", "ldr", "ldr_pseudo", "str", "str_pseudo", 
                           "cmp", "bne", "bge", "bl", "b"};
int latency_num[13] = {0};

void read_latency() {
    string s = "";
    vector <string> temps;
    fin.open("./latency.txt");
    bool comment = 0;

    while (!fin.eof()) {
        char c;
        fin.get(c);

        if (c == '\n') {
            comment = 0;
        } else if (c == '#') {
            comment = 1;
        }
        if (comment) {
            continue;
        }

        if (isalnum(c) || c == '_') {
            s.append(1, c);
        } else if (s.size() >= 1) {
            temps.push_back(s);
            s = "";
        }
    }

    fin.close();

    for (int i = 0; i < temps.size(); i++) {
        for (int j = 0; j < 13; j++) {
            if (temps[i] == latency_insn[j]) {
                latency_num[j] = atoi(temps[i + 1].c_str());
            }
        }
    }
}

int latency_func() {
    vector<int> latency_pipe(5, 0);

    // IF
    latency_pipe[0] = 1;
    // ID
    latency_pipe[1] = 1;
    // EX
    if (Register_ID_EX.opcode > INVALID && Register_ID_EX.opcode <= MOV) {
        latency_pipe[2] = latency_num[Register_ID_EX.opcode - 1];
    } else if (Register_ID_EX.opcode == LDR && Register_ID_EX.type == 3) {
        latency_pipe[2] = latency_num[5];
    } else if (Register_ID_EX.opcode == STR && Register_ID_EX.type == 3) {
        latency_pipe[2] = latency_num[7];
    } else if (Register_ID_EX.opcode >= CMPBNE && Register_ID_EX.opcode <= CMPBGE) {
        latency_pipe[2] = latency_num[8];
    } else {
        latency_pipe[2] = 1;
    }
    // MEM
    if (Register_EX_MEM.opcode == LDR) {
        latency_pipe[3] = latency_num[4];
    } else if (Register_EX_MEM.opcode == STR) {
        latency_pipe[3] = latency_num[6];
    } else if (Register_EX_MEM.opcode >= CMPBNE && Register_EX_MEM.opcode <= B) {
        latency_pipe[3] = latency_num[Register_EX_MEM.opcode + 2];
    } else {
        latency_pipe[3] = 1;
    }
    // WB
    latency_pipe[4] = 1;

    return *max_element(latency_pipe.begin(), latency_pipe.end());
}
// ====================== LATENCY ANALYSIS ======================

void read_instruction() {
    char s;
    char *t;
    int capacity = 10;
    t = new char[capacity];
    int ssize = 0;
    vector <string> temps;
    bool comment = 0;
    fin.open("./instruction.txt");
    while (!fin.eof()) {
        fin.get(s);
        {
            if (int(s) == 64) {
                if (comment == 0)
                    comment = 1;
            } else if (int(s) == 10 && comment == 1) {
                comment = 0;
            }

            if (comment == 0) {
                if (!(s < 123 && int(s) > 96 || int(s) == 39 || int(s) > 64 && int(s) < 91 || int(s) == 46 ||
                      int(s) == 95 || int(s) > 47 && int(s) < 58 || int(s) > 39 && int(s) < 43 || int(s) == 35 ||
                      int(s) == 61)) {
                    if (int(s) == 44 || int(s) == 58 || int(s) == 59 || int(s) == 91 || int(s) == 93) {
                        if (ssize >= 2) {
                            t[ssize] = '\0';
                            lexeme.push_back(t);


                            if (int(s) == 44)
                                lexeme.push_back(",");
                            else if (int(s) == 58) {
                                lexeme.push_back(":");
                            } else if (int(s) == 59)
                                lexeme.push_back(";");
                            else if (int(s) == 91)
                                lexeme.push_back("[");
                            else if (int(s) == 93)
                                lexeme.push_back("]");

                        } else {
                            if (int(s) == 44)
                                lexeme.push_back(",");
                            if (int(s) == 58) {
                                lexeme.push_back(":");
                            }
                            if (int(s) == 59)
                                lexeme.push_back(";");
                            else if (int(s) == 91)
                                lexeme.push_back("[");
                            else if (int(s) == 93)
                                lexeme.push_back("]");
                        }
                    } else {
                        t[ssize] = '\0';
                        if (ssize >= 1) {
                            lexeme.push_back(t);

                        }
                    }
                    ssize = 0;
                    capacity = 10;
                    t = new char[capacity];
                } else {
                    if (ssize < capacity) {
                        t[ssize] = s;
                        ssize++;
                    } else {
                        char *temp = t;
                        t = new char[capacity * 2];
                        capacity = 2 * capacity;
                        for (int i = 0; i < ssize; i++)
                            t[i] = temp[i];
                        t[ssize] = s;
                        ssize++;
                        delete[]temp;
                    }
                }
            }
        }
    }
    fin.close();
}

int main() {

    char s;
    char *t;
    int capacity = 10;
    t = new char[capacity];
    int ssize = 0;
    vector <string> temps;
    int sum = 0;
    int pmax = 0;
    bool comment = 0;
    bool loop_or_data = 0;
    fin.open("./instruction.txt");
    while (!fin.eof()) {
        fin.get(s);
        {
            if (int(s) == 64) {
                if (comment == 0)
                    comment = 1;
            } else if (int(s) == 10 && comment == 1) {
                comment = 0;
            }

            if (comment == 0) {
                if (!(s < 123 && int(s) > 96 || int(s) == 39 || int(s) > 64 && int(s) < 91 || int(s) == 46 ||
                      int(s) == 95 || int(s) > 47 && int(s) < 58 || int(s) > 39 && int(s) < 43 || int(s) == 35 ||
                      int(s) == 61)) {
                    if (int(s) == 44 || int(s) == 58 || int(s) == 59 || int(s) == 91 || int(s) == 93) {
                        if (ssize >= 2) {
                            t[ssize] = '\0';
                            lexeme.push_back(t);


                            if (int(s) == 44)
                                lexeme.push_back(",");
                            else if (int(s) == 58) {
                                lexeme.push_back(":");
                            } else if (int(s) == 59)
                                lexeme.push_back(";");
                            else if (int(s) == 91)
                                lexeme.push_back("[");
                            else if (int(s) == 93)
                                lexeme.push_back("]");

                        } else {
                            if (int(s) == 44)
                                lexeme.push_back(",");
                            if (int(s) == 58) {
                                lexeme.push_back(":");
                            }
                            if (int(s) == 59)
                                lexeme.push_back(";");
                            else if (int(s) == 91)
                                lexeme.push_back("[");
                            else if (int(s) == 93)
                                lexeme.push_back("]");
                        }
                    } else {
                        t[ssize] = '\0';
                        if (ssize >= 1) {
                            lexeme.push_back(t);

                        }
                        if (lexeme.size() > 0) {
                            if (lexeme[lexeme.size() - 1] == ".data") {
                                loop_or_data = 1;
                            } else if (lexeme[lexeme.size() - 1] == ".text") {
                                loop_or_data = 0;
                            }
                        }

                    }
                    sum = 0;
                    ssize = 0;
                    capacity = 10;
                    t = new char[capacity];
                } else {
                    if (ssize < capacity) {
                        t[ssize] = s;
                        ssize++;
                    } else {
                        char *temp = t;
                        t = new char[capacity * 2];
                        capacity = 2 * capacity;
                        for (int i = 0; i < ssize; i++)
                            t[i] = temp[i];
                        t[ssize] = s;
                        ssize++;
                        delete[]temp;
                    }
                }
            }
        }
    }
    fin.close();

    if (make_instruction_file() > 0) {
        return -1;
    }
    read_latency();


    // symbol table
    for (int i = 0; i < symbol_table.size(); ++i) {
        for (int j = 0; j < symbol_temps.size(); ++j) {
            if (symbol_table[i].name == symbol_temps[j].name) {
                if (symbol_temps[j].type == 0) {
                    instruction_file[symbol_temps[j].pos].source = symbol_table[i].pos;
                } else {
                    instruction_file[symbol_temps[j].pos].operand1 = symbol_table[i].pos;
                }
            }
        }
    }

    prog = -1;
    int latency = 0;
    while (shut_down == 0 && end_file_inst == 0) {
        if (Register_MEM_WB.opcode >= 1 && Register_MEM_WB.opcode <= 10)
            instruction_total++;
        cout << BOLD(FGRN("WB")) << "\n";
        WB();
        for (int i = 0; i < 16; ++i) {
            cout << "r[" << i << "]=" << r[i] << "\t";
        }
        cout << "\n";

        cout << BOLD(FRED("MEM")) << endl;
        MEM();

        if (PCSrc == 0) {
            cout << BOLD(FYEL("EX")) << endl;
            EX();

            cout << BOLD(FMAG("ID")) << endl;
            ID();
        } else {
            cout << BOLD(FYEL("EX")) << endl;
            cout << "flush" << endl;
            cout << BOLD(FMAG("ID")) << endl;
            cout << "flush" << endl;
        }
        cout << BOLD(FCYN("IF")) << endl;
        IF();

        latency = latency + 5 * latency_func();
        cout << UNDL(FCYN("latency = ")) << latency << endl << endl;
    }
    cout << "Total instructions are " << instruction_total << " & Total latency is " << latency
         << ", so the CPI will be nearly " << float(latency) / float(instruction_total) << endl;
}
