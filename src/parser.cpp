#include "parser.h"

int lex_idx = 0;


// add / sub / mul / mov / ldr / str
int execute(vector<string> &lexeme, int curr_lex) {
    //Add rd,rop1,rop2
    //cout<<lex_idx<<endl;
    if (lexeme[curr_lex] == "Add" || lexeme[curr_lex] == "add" || lexeme[curr_lex] == "ADD") {
        if ((lexeme[curr_lex + 2] == ",") && (lexeme[curr_lex + 4] == ",")) {
            //cout<<"Error";
            //Add rd,rop1,rop2;
            if ((lexeme[curr_lex + 1][0] == 'r') && (lexeme[curr_lex + 3][0] == 'r') &&
                (lexeme[curr_lex + 5][0] == 'r')) {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP &&
                    stoi(lexeme[curr_lex + 5].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            }
                //Add rd,rop1,#Imm
            else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r' &&
                     lexeme[curr_lex + 5][0] == '#') {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            }
                //Add SP,SP,#Imm;
            else if ((lexeme[curr_lex + 1] == "SP" || lexeme[curr_lex + 1] == "sp") &&
                     (lexeme[curr_lex + 3] == "SP" || lexeme[curr_lex + 3] == "sp") && lexeme[curr_lex + 5][0] == '#') {
                lex_idx = curr_lex + 6;
                return 0;
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 6;
            return -1;
        }

        //';' excluded from every instruction

    }

        //Variation of subtraction
    else if (lexeme[curr_lex] == "Sub" || lexeme[curr_lex] == "sub" || lexeme[curr_lex] == "SUB") {
        if ((lexeme[curr_lex + 2] == ",") && (lexeme[curr_lex + 4] == ",")) {
            //cout<<"Error";
            //Sub rd,rop1,rop2;
            if ((lexeme[curr_lex + 1][0] == 'r') && (lexeme[curr_lex + 3][0] == 'r') &&
                (lexeme[curr_lex + 5][0] == 'r')) {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP &&
                    stoi(lexeme[curr_lex + 5].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            }
                //Sub rd,rop1,#Imm;
            else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r' &&
                     lexeme[curr_lex + 5][0] == '#') {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            }
                //Sub SP,SP,#Imm;
            else if ((lexeme[curr_lex + 1] == "SP" || lexeme[curr_lex + 1] == "sp") &&
                     (lexeme[curr_lex + 3] == "SP" || lexeme[curr_lex + 3] == "sp") && lexeme[curr_lex + 5][0] == '#') {
                lex_idx = curr_lex + 6;
                return 0;
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 6;
            return -1;
        }
        //lex_idx = curr_lex+6;
    }

        //variation of mul instruction
    else if (lexeme[curr_lex] == "Mul" || lexeme[curr_lex] == "mul" || lexeme[curr_lex] == "MUL") {
        if ((lexeme[curr_lex + 2] == ",") && (lexeme[curr_lex + 4] == ",")) {
            //cout<<"Error";
            //Mul rd,rop1,rop2
            if ((lexeme[curr_lex + 1][0] == 'r') && (lexeme[curr_lex + 3][0] == 'r') &&
                (lexeme[curr_lex + 5][0] == 'r')) {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP &&
                    stoi(lexeme[curr_lex + 5].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            }
                //Mul rd,rop1,#Imm
            else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r' &&
                     lexeme[curr_lex + 5][0] == '#') {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 6;
            return -1;
        }
        //lex_idx = curr_lex+6;
    } else if (lexeme[curr_lex] == "Mov" || lexeme[curr_lex] == "mov" || lexeme[curr_lex] == "MOV") {
        if ((lexeme[curr_lex + 2] == ",")) {
            //Mov r1,#Imm
            if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == '#') {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 4;
                    return 0;
                } else {
                    lex_idx = curr_lex + 4;
                    return -1;
                }

            }
                //Mov rop1,rop2
            else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r') {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 4;
                    return 0;
                } else {
                    lex_idx = curr_lex + 4;
                    return -1;
                }
                // lex_idx = curr_lex+4;
            }
                //Mov curr_lex,lr
            else if ((lexeme[curr_lex + 1] == "pc") && lexeme[curr_lex + 3] == "lr") {
                lex_idx = curr_lex + 4;
                return 0;
            } else {
                lex_idx = curr_lex + 4;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 4;
            return -1;

        }
        //lex_idx = curr_lex+4;
    }

        //memory instructions
    else if (lexeme[curr_lex] == "Ldr" || lexeme[curr_lex] == "ldr" || lexeme[curr_lex] == "LDR") {
        //Ldr rd,=label
        if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == '=') {
            lex_idx = curr_lex + 4;
            return 0;
        }

            //Ldr rd,[rop1]
        else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 4][0] == 'r' && lexeme[curr_lex + 5] == "]") {
            if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 5] == "]") {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 4].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
            //lex_idx = curr_lex + 6;
        }

            //Ldr rd,[rop1,#Imm]
        else if ((lexeme[curr_lex + 1][0] == 'r') && (lexeme[curr_lex + 4][0] == 'r') &&
                 (lexeme[curr_lex + 6][0] == '#')) {
            if ((lexeme[curr_lex + 2] == ",")) {
                if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 7] == "]" && (lexeme[curr_lex + 5] == ",")) {
                    if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP &&
                        stoi(lexeme[curr_lex + 4].substr(1)) < REG_SP) {
                        lex_idx = curr_lex + 8;
                        return 0;
                    } else {
                        lex_idx = curr_lex + 8;
                        return -1;
                    }
                } else {
                    lex_idx = curr_lex + 8;
                    return -1;
                }
            } else {
                lex_idx = curr_lex + 8;
                return -1;
            }
        }


            //Ldr LR,[SP,#Imm]
            //Ldr rop1,[SP,#Imm]
        else if ((lexeme[curr_lex + 4] == "SP" || lexeme[curr_lex + 4] == "sp") && lexeme[curr_lex + 6][0] == '#' &&
                 lexeme[curr_lex + 2] == "," && lexeme[curr_lex + 5] == ",") {
            if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 7] == "]") {
                string s = lexeme[curr_lex + 1].substr(1);
                if (lexeme[curr_lex + 1][0] == 'r' && stoi(s) < REG_SP) {
                    lex_idx = curr_lex + 8;
                    return 0;
                } else if (lexeme[curr_lex + 1] == "LR" || lexeme[curr_lex + 1] == "lr") {
                    lex_idx = curr_lex + 8;
                    return 0;
                } else {
                    lex_idx = curr_lex + 8;
                    return -1;
                }
                //lex_idx=curr_lex+8;
            } else {
                lex_idx = curr_lex + 8;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 8;
            return -1;
        }
    }

        //store instruction

    else if (lexeme[curr_lex] == "Str" || lexeme[curr_lex] == "str" || lexeme[curr_lex] == "STR") {
        //Ldr rd,=label
        if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == '=') {
            lex_idx = curr_lex + 4;
            return 0;
        }
            //Ldr rd,[rop1]
        else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 4][0] == 'r' && lexeme[curr_lex + 5] == "]") {
            if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 5] == "]") {
                if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP && stoi(lexeme[curr_lex + 4].substr(1)) < REG_SP) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
            //lex_idx = curr_lex + 6;
        }
            //Str rd,[rop1,#Imm]
        else if ((lexeme[curr_lex + 1][0] == 'r') && (lexeme[curr_lex + 4][0] == 'r') &&
                 (lexeme[curr_lex + 6][0] == '#')) {
            if ((lexeme[curr_lex + 2] == ",")) {
                if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 7] == "]" && (lexeme[curr_lex + 5] == ",")) {
                    if (stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP &&
                        stoi(lexeme[curr_lex + 4].substr(1)) < REG_SP) {
                        lex_idx = curr_lex + 8;
                        return 0;
                    } else {
                        lex_idx = curr_lex + 8;
                        return -1;
                    }
                } else {
                    lex_idx = curr_lex + 8;
                    return -1;
                }
            } else {
                lex_idx = curr_lex + 8;
                return -1;
            }
        }


            //Ldr LR,[SP,#Imm]
            //Ldr rop1,[SP,#Imm]
        else if ((lexeme[curr_lex + 4] == "SP" || lexeme[curr_lex + 4] == "sp") && lexeme[curr_lex + 6][0] == '#' &&
                 lexeme[curr_lex + 2] == "," && lexeme[curr_lex + 5] == ",") {
            if (lexeme[curr_lex + 3] == "[" && lexeme[curr_lex + 7] == "]") {
                string s = lexeme[curr_lex + 1].substr(1);
                if (lexeme[curr_lex + 1][0] == 'r' && stoi(s) < REG_SP) {
                    lex_idx = curr_lex + 8;
                    return 0;
                } else if (lexeme[curr_lex + 1] == "LR" || lexeme[curr_lex + 1] == "lr") {
                    lex_idx = curr_lex + 8;
                    return 0;
                } else {
                    lex_idx = curr_lex + 8;
                    return -1;
                }
                //lex_idx=curr_lex+8;
            } else {
                lex_idx = curr_lex + 8;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 8;
            return -1;
        }
    }
    return -1;
}

// cmp & bne / cmp & bge / bl / b
int compare(vector<string> &lexeme, int curr_lex) {
    //Cmp rop1,rop2
    //Bne label
    //cout<<"hello"<<lex_idx<<endl;
    if (lexeme[curr_lex + 4] == "Bne" || lexeme[curr_lex + 4] == "bne" || lexeme[curr_lex + 4] == "BNE") {
        if (lexeme[curr_lex + 2] == ",") {
            if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r') {
                if ((stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP) &&
                    (stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP)) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            } else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == '#') {
                lex_idx = curr_lex + 6;
                return 0;
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 6;
            return -1;
        }
    }

        //Cmp rop1,rop2
        //Bge label
    else if (lexeme[curr_lex + 4] == "Bge" || lexeme[curr_lex + 4] == "bge" || lexeme[curr_lex + 4] == "BGE") {
        if (lexeme[curr_lex + 2] == ",") {
            if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == 'r') {
                if ((stoi(lexeme[curr_lex + 1].substr(1)) < REG_SP) &&
                    (stoi(lexeme[curr_lex + 3].substr(1)) < REG_SP)) {
                    lex_idx = curr_lex + 6;
                    return 0;
                } else {
                    lex_idx = curr_lex + 6;
                    return -1;
                }
            } else if (lexeme[curr_lex + 1][0] == 'r' && lexeme[curr_lex + 3][0] == '#') {
                lex_idx = curr_lex + 6;
                return 0;
            } else {
                lex_idx = curr_lex + 6;
                return -1;
            }
        } else {
            lex_idx = curr_lex + 6;
            return -1;
        }
    } else {
        lex_idx = curr_lex + 6;
        return -1;
    }
}


// ====================== LATENCY PARSING ======================
int parse_latency(vector<string> &lexeme, vector<int> &latencies) {
    int error = 0;

    const string latency_instr[12] = {
            "add", "sub", "mul", "mov",
            "ldr", "ldr_pseudo", "str",
            "cmp", "bne", "bge",
            "bl", "b"
    };

    for (int i = 0; i < 12; i++) {
        int instr_found = 0;
        for (int j = 0; j < lexeme.size(); j++) {
            if (latency_instr[i] == lexeme[j]) {
                latencies[i] = stoi(lexeme[j + 1]);
                instr_found = 1;
            }
        }
        if (!instr_found) {
            cout << "Error in " << latency_instr[i] << " latency" << endl;
            error++;
        }
    }

    return error;
}

int read_latency(vector<int> &latencies) {
    vector<string> lexeme;
    string s;
    int comment = 0;

    ifstream fin;
    fin.open(latency_path);
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
        } else if (!s.empty()) {
            lexeme.push_back(s);
            s = "";
        }
    }
    fin.close();

    return parse_latency(lexeme, latencies);
}
// ====================== LATENCY PARSING ======================


// ==================== INSTRUCTION PARSING ====================
int parse_instruction(vector<string> lexeme, vector<Instruction> &instructions) {
    int error = 0;

    int freemem = 5000;
    vector<symbol> symbol_table;
    vector<symbol> symbol_temps;

    while (lex_idx < lexeme.size()) {
        string lex_lower = lexeme[lex_idx];
        transform(lex_lower.begin(), lex_lower.end(), lex_lower.begin(), ::tolower);
        if (lexeme[lex_idx] == "add" || lexeme[lex_idx] == "sub" || lexeme[lex_idx] == "mul" ||
            lexeme[lex_idx] == "mov" || lexeme[lex_idx] == "ldr" || lexeme[lex_idx] == "str") {
            int p1 = lex_idx;
            int p = execute(lexeme, lex_idx);
            lex_idx = p1;
            if (p == -1) {
                cout << "Error in " << lexeme[lex_idx] << " instruction" << endl;
                lex_idx++;
                error++;
            } else {

                if (lexeme[lex_idx] == "Add" || lexeme[lex_idx] == "add" || lexeme[lex_idx] == "ADD")       //Addition
                {
                    Instruction temp_i;
                    temp_i.opcode = OPC_ADD;

                    //Add rd,rop1,rop2
                    if (lexeme[lex_idx + 5][0] == 'r') {
                        temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                        temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                        temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                        temp_i.type = 1;
                    } else {

                        //Add rd ,rop1,#Imm
                        if (lexeme[lex_idx + 3][0] == 'r') {
                            temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                            temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                            temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if (lexeme[lex_idx + 3] == "SP" || lexeme[lex_idx + 3] == "Sp" ||
                                 lexeme[lex_idx + 3] == "sp") {
                            temp_i.source = REG_SP;
                            temp_i.operand1 = REG_SP;
                            temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instructions.push_back(temp_i);
                    lex_idx += 6;
                } else if (lexeme[lex_idx] == "Sub" || lexeme[lex_idx] == "sub" ||
                           lexeme[lex_idx] == "SUB")      //Subtract
                {
                    Instruction temp_i;
                    temp_i.opcode = OPC_SUB;

                    //Sub rd,rop1,rop2
                    if (lexeme[lex_idx + 5][0] == 'r') {
                        temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                        temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                        temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                        temp_i.type = 1;
                    } else {

                        //Add rd ,rop1,#Imm
                        if (lexeme[lex_idx + 3][0] == 'r') {
                            temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                            temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                            temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if (lexeme[lex_idx + 3] == "SP" || lexeme[lex_idx + 3] == "Sp" ||
                                 lexeme[lex_idx + 3] == "sp") {
                            temp_i.source = REG_SP;
                            temp_i.operand1 = REG_SP;
                            temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instructions.push_back(temp_i);
                    lex_idx += 6;
                } else if (lexeme[lex_idx] == "Mul" || lexeme[lex_idx] == "mul" ||
                           lexeme[lex_idx] == "MUL")      //Multiply
                {
                    Instruction temp_i;
                    temp_i.opcode = OPC_MUL;
                    temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                    temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                    //Mul rd,rop1,rop2
                    if (lexeme[lex_idx + 5][0] == 'r') {
                        temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                        temp_i.type = 1;
                    }
                        //Mul rd, rop1,#Imm
                    else {
                        temp_i.operand2 = stoi(lexeme[lex_idx + 5].substr(1));
                        temp_i.type = 2;
                    }
                    instructions.push_back(temp_i);
                    lex_idx += 6;
                } else if (lexeme[lex_idx] == "Mov" || lexeme[lex_idx] == "mov" || lexeme[lex_idx] == "MOV")      //Move
                {
                    Instruction temp_i;

                    temp_i.opcode = OPC_MOV;
                    if ((lexeme[lex_idx + 1] == "pc" || lexeme[lex_idx + 1] == "Pc" || lexeme[lex_idx + 1] == "PC") &&
                        (lexeme[lex_idx + 3] == "lr" || lexeme[lex_idx + 3] == "Lr" || lexeme[lex_idx + 3] == "LR")) {
                        temp_i.source = -1;
                        temp_i.operand1 = -1;
                        temp_i.type = 3;
                        instructions.push_back(temp_i);
                    } else {
                        temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                        temp_i.operand2 = -1;
                        if (lexeme[lex_idx + 3][0] == 'r')  //Mov rd rop1
                        {
                            temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                            temp_i.type = 1;
                        } else//Mov rd #12
                        {
                            temp_i.operand1 = stoi(lexeme[lex_idx + 3].substr(1));
                            temp_i.type = 2;
                        }
                        instructions.push_back(temp_i);
                    }
                    lex_idx += 4;

                } else if (lexeme[lex_idx] == "Ldr" || lexeme[lex_idx] == "ldr" ||
                           lexeme[lex_idx] == "LDR")          //Load
                {
                    Instruction temp_i;
                    temp_i.opcode = OPC_LDR;
                    if (lexeme[lex_idx + 1] == "LR" || lexeme[lex_idx + 1] == "Lr" || lexeme[lex_idx + 1] == "lr") {
                        temp_i.source = REG_LR;
                    } else {
                        temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                    }

                    if (lexeme[lex_idx + 3][0] == '[')      //Ldr rd, [rop1]
                    {
                        if (lexeme[lex_idx + 5][0] == ']') {
                            //ldr rd,[sp]
                            if (lexeme[lex_idx + 4] == "SP" || lexeme[lex_idx + 4] == "Sp" ||
                                lexeme[lex_idx + 4] == "sp") {
                                temp_i.operand1 = REG_SP;
                                temp_i.operand2 = -1;
                                temp_i.type = 1;
                                lex_idx += 6;
                            } else {
                                temp_i.operand1 = stoi(lexeme[lex_idx + 4].substr(1));
                                temp_i.operand2 = -1;
                                temp_i.type = 1;
                                lex_idx += 6;
                            }
                        } else {
                            //ldr rd,[sp,#IMM]
                            if (lexeme[lex_idx + 4] == "SP" || lexeme[lex_idx + 4] == "Sp" ||
                                lexeme[lex_idx + 4] == "sp") {
                                temp_i.operand1 = REG_SP;
                                temp_i.operand2 = stoi(lexeme[lex_idx + 6].substr(1));
                                temp_i.type = 2;
                                lex_idx += 8;
                            } else {
                                temp_i.operand1 = stoi(lexeme[lex_idx + 4].substr(1));
                                temp_i.operand2 = stoi(lexeme[lex_idx + 6].substr(1));
                                temp_i.type = 2;
                                lex_idx += 8;
                            }
                        }
                    } else if (lexeme[lex_idx + 3][0] == '=')      //Ldr rd, =AA
                    {
                        temp_i.operand1 = -2;
                        symbol tem;                                       //change
                        tem.name = lexeme[lex_idx + 3].substr(1);
                        tem.pos = instructions.size();
                        tem.type = 1;
                        symbol_temps.push_back(tem);
                        temp_i.operand2 = -1;
                        temp_i.type = 3;
                        lex_idx += 4;                                                    //change
                    }
                    instructions.push_back(temp_i);
                } else        //Store
                {
                    Instruction temp_i;
                    temp_i.opcode = OPC_STR;

                    if (lexeme[lex_idx + 1] == "LR" || lexeme[lex_idx + 1] == "Lr" || lexeme[lex_idx + 1] == "lr") {
                        temp_i.source = REG_LR;
                    } else {
                        temp_i.source = stoi(lexeme[lex_idx + 1].substr(1));
                    }
                    if (lexeme[lex_idx + 4][0] == 'r' && lexeme[lex_idx + 5][0] == ']')      //Str rd, [rop1]
                    {
                        if (lexeme[lex_idx + 4] == "SP" || lexeme[lex_idx + 4] == "Sp" || lexeme[lex_idx + 4] == "sp") {
                            temp_i.operand1 = REG_SP;
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            lex_idx += 6;
                        } else {
                            temp_i.operand1 = stoi(lexeme[lex_idx + 4].substr(1));
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            lex_idx += 6;
                        }
                    } else        //Str rd, [rop1, #21]
                    {
                        if (lexeme[lex_idx + 4] == "SP" || lexeme[lex_idx + 4] == "Sp" || lexeme[lex_idx + 4] == "sp") {
                            temp_i.operand1 = REG_SP;
                            temp_i.operand2 = stoi(lexeme[lex_idx + 6].substr(1));
                            temp_i.type = 2;
                            lex_idx += 8;
                        } else {
                            temp_i.operand1 = stoi(lexeme[lex_idx + 4].substr(1));
                            temp_i.operand2 = stoi(lexeme[lex_idx + 6].substr(1));
                            temp_i.type = 2;
                            lex_idx += 8;
                        }
                    }
                    instructions.push_back(temp_i);
                }
            }

        } else if (lexeme[lex_idx] == "Cmp" || lexeme[lex_idx] == "cmp" || lexeme[lex_idx] == "CMP")         //compare
        {
            int q1 = lex_idx;
            int q = compare(lexeme, lex_idx);
            lex_idx = q1;
            if (q == -1) {
                cout << "Error in cmp instruction" << endl;
                lex_idx++;
                error++;
            } else if (lexeme[lex_idx + 4] == "Bne" || lexeme[lex_idx + 4] == "bne" ||
                       lexeme[lex_idx + 4] == "BNE")      //cmp and bne
            {
                Instruction temp_i;
                temp_i.opcode = OPC_CMPBNE;
                temp_i.source = -2;                             //change
                symbol tem;                                   //change
                tem.name = lexeme[lex_idx + 5];
                tem.pos = instructions.size();
                symbol_temps.push_back(tem);
                if (lexeme[lex_idx + 1][0] == 'r' && lexeme[lex_idx + 3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[lex_idx + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[lex_idx + 3].substr(1));                                   //change
                    temp_i.type = 1;
                    lex_idx += 6;
                } else if (lexeme[lex_idx + 1][0] == 'r' && lexeme[lex_idx + 3][0] == '#')        //cmp rd, #5
                {
                    temp_i.operand1 = stoi(lexeme[lex_idx + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[lex_idx + 3].substr(1));                                   //change
                    temp_i.type = 2;
                    lex_idx += 6;
                }
                instructions.push_back(temp_i);
            } else if (lexeme[lex_idx + 4] == "Bge" || lexeme[lex_idx + 4] == "bge" ||
                       lexeme[lex_idx + 4] == "BGE")         //cmp and bge
            {
                Instruction temp_i;
                temp_i.opcode = OPC_CMPBGE;
                temp_i.source = -2;                                                             //change
                symbol tem;                                                                       //change
                tem.name = lexeme[lex_idx + 5];
                tem.pos = instructions.size();
                symbol_temps.push_back(tem);
                if (lexeme[lex_idx + 1][0] == 'r' && lexeme[lex_idx + 3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[lex_idx + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[lex_idx + 3].substr(1));
                    temp_i.type = 1;
                    lex_idx += 6;
                } else if (lexeme[lex_idx + 1][0] == 'r' && lexeme[lex_idx + 3][0] == '#')        //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[lex_idx + 1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[lex_idx + 3].substr(1));
                    temp_i.type = 2;
                    lex_idx += 6;
                }
                instructions.push_back(temp_i);
            }

        } else if (lexeme[lex_idx] == "BL" || lexeme[lex_idx] == "bl" || lexeme[lex_idx] == "Bl")     //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = OPC_BL;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symbol tem;                                                                   //change
            tem.name = lexeme[lex_idx + 1];
            tem.pos = instructions.size();
            symbol_temps.push_back(tem);
            lex_idx += 2;
            instructions.push_back(temp_i);
        } else if (lexeme[lex_idx] == "B" || lexeme[lex_idx] == "b")        //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = OPC_B;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symbol tem;                                                                   //change
            tem.name = lexeme[lex_idx + 1];
            tem.pos = instructions.size();
            symbol_temps.push_back(tem);
            lex_idx += 2;
            instructions.push_back(temp_i);
        } else if (lexeme[lex_idx] == ":") {
            symbol tem;
            tem.name = lexeme[lex_idx - 1];
            if (lexeme[lex_idx + 1] == ".space") {
                tem.pos = freemem;
                freemem = freemem + stoi(lexeme[lex_idx + 2]);
                tem.type = 2;
            } else {
                tem.pos = instructions.size();
            }
            symbol_table.push_back(tem);
            lex_idx++;
        } else if (lexeme[lex_idx] == "Exit" || lexeme[lex_idx] == "exit" || lexeme[lex_idx] == "EXIT" ||
                   lexeme[lex_idx] == "Swi_Exit" || lexeme[lex_idx] == "swi_exit" || lexeme[lex_idx] == "SWI_EXIT") {
            Instruction temp_i;
            temp_i.opcode = OPC_EXIT;
            temp_i.source = -1;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = -1;
            instructions.push_back(temp_i);
            //symbol tem;
            lex_idx++;
        } else lex_idx++;
    }

    symbol tem_e;
    tem_e.name = "Exit";
    tem_e.pos = instructions.size();
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

    for (int i = 0; i < symbol_table.size(); ++i) {
        for (int j = 0; j < symbol_temps.size(); ++j) {
            if (symbol_table[i].name == symbol_temps[j].name) {
                if (symbol_temps[j].type == 0) {
                    instructions[symbol_temps[j].pos].source = symbol_table[i].pos;
                } else {
                    instructions[symbol_temps[j].pos].operand1 = symbol_table[i].pos;
                }
            }
        }
    }

    return error;
}

int read_instruction(vector<Instruction> &instructions) {
    vector<string> lexeme;
    string s;
    int comment = 0;

    ifstream fin;
    fin.open(instruction_path);
    while (!fin.eof()) {
        char c;
        fin.get(c);

        if (c == '\n') {
            comment = 0;
        } else if (c == '@') {
            comment = 1;
        }
        if (comment) {
            continue;
        }

        if (isalnum(c) || c == '_' || c == '.' || c == '#' || c == '=') {
            s.append(1, c);
        } else {
            if (!s.empty()) {
                lexeme.push_back(s);
                s = "";
            }
            if (c == ':' || c == ',' || c == '[' || c == ']') {
                lexeme.emplace_back(1, c);
            }
        }
    }
    fin.close();

    return parse_instruction(lexeme, instructions);
}
// ==================== INSTRUCTION PARSING ====================


int parse_file(vector<int> &latencies, vector<Instruction> &instructions) {
    int err = 0;
    err += read_latency(latencies);
    err += read_instruction(instructions);
    return err;
}
