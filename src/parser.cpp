#include "parser.h"

int execute(int pc) {
    //Add rd,rop1,rop2
    //cout<<prog<<endl;
    if (lexeme[pc] == "Add" || lexeme[pc] == "add" || lexeme[pc] == "ADD") {
        if ((lexeme[pc + 2] == ",") && (lexeme[pc + 4] == ",")) {
            //cout<<"Error";
            //Add rd,rop1,rop2;
            if ((lexeme[pc + 1][0] == 'r') && (lexeme[pc + 3][0] == 'r') && (lexeme[pc + 5][0] == 'r')) {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13 &&
                    stoi(lexeme[pc + 5].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            }
                //Add rd,rop1,#Imm
            else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r' && lexeme[pc + 5][0] == '#') {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            }
                //Add SP,SP,#Imm;
            else if ((lexeme[pc + 1] == "SP" || lexeme[pc + 1] == "sp") &&
                     (lexeme[pc + 3] == "SP" || lexeme[pc + 3] == "sp") && lexeme[pc + 5][0] == '#') {
                prog = pc + 6;
                return 0;
            } else {
                prog = pc + 6;
                return -1;
            }
        } else {
            prog = pc + 6;
            return -1;
        }

        //';' excluded from every instruction

    }

        //Variation of subtraction
    else if (lexeme[pc] == "Sub" || lexeme[pc] == "sub" || lexeme[pc] == "SUB") {
        if ((lexeme[pc + 2] == ",") && (lexeme[pc + 4] == ",")) {
            //cout<<"Error";
            //Sub rd,rop1,rop2;
            if ((lexeme[pc + 1][0] == 'r') && (lexeme[pc + 3][0] == 'r') && (lexeme[pc + 5][0] == 'r')) {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13 &&
                    stoi(lexeme[pc + 5].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            }
                //Sub rd,rop1,#Imm;
            else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r' && lexeme[pc + 5][0] == '#') {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            }
                //Sub SP,SP,#Imm;
            else if ((lexeme[pc + 1] == "SP" || lexeme[pc + 1] == "sp") &&
                     (lexeme[pc + 3] == "SP" || lexeme[pc + 3] == "sp") && lexeme[pc + 5][0] == '#') {
                prog = pc + 6;
                return 0;
            } else {
                prog = pc + 6;
                return -1;
            }
        } else {
            prog = pc + 6;
            return -1;
        }
        //prog = pc+6;
    }

        //variation of mul instruction
    else if (lexeme[pc] == "Mul" || lexeme[pc] == "mul" || lexeme[pc] == "MUL") {
        if ((lexeme[pc + 2] == ",") && (lexeme[pc + 4] == ",")) {
            //cout<<"Error";
            //Mul rd,rop1,rop2
            if ((lexeme[pc + 1][0] == 'r') && (lexeme[pc + 3][0] == 'r') && (lexeme[pc + 5][0] == 'r')) {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13 &&
                    stoi(lexeme[pc + 5].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            }
                //Mul rd,rop1,#Imm
            else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r' && lexeme[pc + 5][0] == '#') {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            } else {
                prog = pc + 6;
                return -1;
            }
        } else {
            prog = pc + 6;
            return -1;
        }
        //prog = pc+6;
    } else if (lexeme[pc] == "Mov" || lexeme[pc] == "mov" || lexeme[pc] == "MOV") {
        if ((lexeme[pc + 2] == ",")) {
            //Mov r1,#Imm
            if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == '#') {
                if (stoi(lexeme[pc + 1].substr(1)) < 13) {
                    prog = pc + 4;
                    return 0;
                } else {
                    prog = pc + 4;
                    return -1;
                }

            }
                //Mov rop1,rop2
            else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r') {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 3].substr(1)) < 13) {
                    prog = pc + 4;
                    return 0;
                } else {
                    prog = pc + 4;
                    return -1;
                }
                // prog = pc+4;
            }
                //Mov pc,lr
            else if ((lexeme[pc + 1] == "pc") && lexeme[pc + 3] == "lr") {
                prog = pc + 4;
                return 0;
            } else {
                prog = pc + 4;
                return -1;
            }
        } else {
            prog = pc + 4;
            return -1;

        }
        //prog = pc+4;
    }

        //memory instructions
    else if (lexeme[pc] == "Ldr" || lexeme[pc] == "ldr" || lexeme[pc] == "LDR") {
        //Ldr rd,=label
        if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == '=') {
            prog = pc + 4;
            return 0;
        }

            //Ldr rd,[rop1]
        else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 4][0] == 'r' && lexeme[pc + 5] == "]") {
            if (lexeme[pc + 3] == "[" && lexeme[pc + 5] == "]") {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 4].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            } else {
                prog = pc + 6;
                return -1;
            }
            //prog = pc + 6;
        }

            //Ldr rd,[rop1,#Imm]
        else if ((lexeme[pc + 1][0] == 'r') && (lexeme[pc + 4][0] == 'r') && (lexeme[pc + 6][0] == '#')) {
            if ((lexeme[pc + 2] == ",")) {
                if (lexeme[pc + 3] == "[" && lexeme[pc + 7] == "]" && (lexeme[pc + 5] == ",")) {
                    if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 4].substr(1)) < 13) {
                        prog = pc + 8;
                        return 0;
                    } else {
                        prog = pc + 8;
                        return -1;
                    }
                } else {
                    prog = pc + 8;
                    return -1;
                }
            } else {
                prog = pc + 8;
                return -1;
            }
        }


            //Ldr LR,[SP,#Imm]
            //Ldr rop1,[SP,#Imm]
        else if ((lexeme[pc + 4] == "SP" || lexeme[pc + 4] == "sp") && lexeme[pc + 6][0] == '#' &&
                 lexeme[pc + 2] == "," && lexeme[pc + 5] == ",") {
            if (lexeme[pc + 3] == "[" && lexeme[pc + 7] == "]") {
                string s = lexeme[pc + 1].substr(1);
                if (lexeme[pc + 1][0] == 'r' && stoi(s) < 13) {
                    prog = pc + 8;
                    return 0;
                } else if (lexeme[pc + 1] == "LR" || lexeme[pc + 1] == "lr") {
                    prog = pc + 8;
                    return 0;
                } else {
                    prog = pc + 8;
                    return -1;
                }
                //prog=pc+8;
            } else {
                prog = pc + 8;
                return -1;
            }
        } else {
            prog = pc + 8;
            return -1;
        }
    }

        //store instruction

    else if (lexeme[pc] == "Str" || lexeme[pc] == "str" || lexeme[pc] == "STR") {
        //Ldr rd,=label
        if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == '=') {
            prog = pc + 4;
            return 0;
        }
            //Ldr rd,[rop1]
        else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 4][0] == 'r' && lexeme[pc + 5] == "]") {
            if (lexeme[pc + 3] == "[" && lexeme[pc + 5] == "]") {
                if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 4].substr(1)) < 13) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            } else {
                prog = pc + 6;
                return -1;
            }
            //prog = pc + 6;
        }
            //Str rd,[rop1,#Imm]
        else if ((lexeme[pc + 1][0] == 'r') && (lexeme[pc + 4][0] == 'r') && (lexeme[pc + 6][0] == '#')) {
            if ((lexeme[pc + 2] == ",")) {
                if (lexeme[pc + 3] == "[" && lexeme[pc + 7] == "]" && (lexeme[pc + 5] == ",")) {
                    if (stoi(lexeme[pc + 1].substr(1)) < 13 && stoi(lexeme[pc + 4].substr(1)) < 13) {
                        prog = pc + 8;
                        return 0;
                    } else {
                        prog = pc + 8;
                        return -1;
                    }
                } else {
                    prog = pc + 8;
                    return -1;
                }
            } else {
                prog = pc + 8;
                return -1;
            }
        }


            //Ldr LR,[SP,#Imm]
            //Ldr rop1,[SP,#Imm]
        else if ((lexeme[pc + 4] == "SP" || lexeme[pc + 4] == "sp") && lexeme[pc + 6][0] == '#' &&
                 lexeme[pc + 2] == "," && lexeme[pc + 5] == ",") {
            if (lexeme[pc + 3] == "[" && lexeme[pc + 7] == "]") {
                string s = lexeme[pc + 1].substr(1);
                if (lexeme[pc + 1][0] == 'r' && stoi(s) < 13) {
                    prog = pc + 8;
                    return 0;
                } else if (lexeme[pc + 1] == "LR" || lexeme[pc + 1] == "lr") {
                    prog = pc + 8;
                    return 0;
                } else {
                    prog = pc + 8;
                    return -1;
                }
                //prog=pc+8;
            } else {
                prog = pc + 8;
                return -1;
            }
        } else {
            prog = pc + 8;
            return -1;
        }
    }
    return -1;
}

//not working


//branch instruction
int compare(int pc) {
    //Cmp rop1,rop2
    //Bne label
    //cout<<"hello"<<prog<<endl;
    if (lexeme[pc + 4] == "Bne" || lexeme[pc + 4] == "bne" || lexeme[pc + 4] == "BNE") {
        if (lexeme[pc + 2] == ",") {
            if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r') {
                if ((stoi(lexeme[pc + 1].substr(1)) < 13) && (stoi(lexeme[pc + 3].substr(1)) < 13)) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            } else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == '#') {
                prog = pc + 6;
                return 0;
            } else {
                prog = pc + 6;
                return -1;
            }
        } else {
            prog = pc + 6;
            return -1;
        }
    }

        //Cmp rop1,rop2
        //Bge label
    else if (lexeme[pc + 4] == "Bge" || lexeme[pc + 4] == "bge" || lexeme[pc + 4] == "BGE") {
        if (lexeme[pc + 2] == ",") {
            if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == 'r') {
                if ((stoi(lexeme[pc + 1].substr(1)) < 13) && (stoi(lexeme[pc + 3].substr(1)) < 13)) {
                    prog = pc + 6;
                    return 0;
                } else {
                    prog = pc + 6;
                    return -1;
                }
            } else if (lexeme[pc + 1][0] == 'r' && lexeme[pc + 3][0] == '#') {
                prog = pc + 6;
                return 0;
            } else {
                prog = pc + 6;
                return -1;
            }
        } else {
            prog = pc + 6;
            return -1;
        }
    } else {
        prog = pc + 6;
        return -1;
    }
}
