#include "pipeline_sim.h"

ifstream fin;
vector<string> lexeme;
int prog=0;
int mem[6000];
int freemem = 5000;
struct loop
{
    int loop_no;
    string loop_name;
};
struct loop1
{
    int loop_no;
    string loop_name;
    int start;
};
struct symb         //change
{
    string name;
    int pos;
    int type = 0;
};

vector<symb> symbol_table;
vector<symb> temp_symb;

struct Instruction      //change
{
    int opcode = -11;
    int source = -11;
    int operand1 = -11;
    int operand2 = -11;
    int type = -11;
};

int shut_down = 0;
bool branch_predictor_bit[2] = {0};

vector<Instruction> instruction_file;       //change
vector<loop> loops;     //change
vector<loop1> datas;
string exit_name = "Exit";
int inst_size = 0;
int r[16] = {0};
int end_file_inst = 0;

int errorHandler=0;


//###########################INSTRUCTION FILE######################################
void make_instruction_file()
{

    while(prog < lexeme.size())
    {
        //cout<<prog<<endl;
        if(lexeme[prog]=="Add"||lexeme[prog]=="add"||lexeme[prog]=="ADD"||lexeme[prog]=="Sub"||lexeme[prog]=="sub"||lexeme[prog]=="SUB"||lexeme[prog]=="Mul"||lexeme[prog]=="mul"||lexeme[prog]=="MUL"||lexeme[prog]=="Mov"||lexeme[prog]=="mov"||lexeme[prog]=="MOV"||lexeme[prog]=="Ldr"||lexeme[prog]=="ldr"||lexeme[prog]=="LDR"||lexeme[prog]=="Str"||lexeme[prog]=="str"||lexeme[prog]=="STR")
        {

            int p1 = prog;
            //int p=1;//
            int p=execute(prog);
            //int q=compare(prog);
            prog = p1;
            if(p==-1 )
            {
                cout<<"Error in "<<lexeme[prog]<<" instruction"<<endl;
                prog++;
                errorHandler++;
            }
            else
            {

                if(lexeme[prog]=="Add"||lexeme[prog]=="add"||lexeme[prog]=="ADD")       //Addition
                {
                    Instruction temp_i;
                    temp_i.opcode = 1;

                    //Add rd,rop1,rop2
                    if(lexeme[prog+5][0]=='r')
                    {
                        temp_i.source = stoi(lexeme[prog+1].substr(1));
                        temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                        temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                        temp_i.type = 1;
                    }

                    else
                    {

                        //Add rd ,rop1,#Imm
                        if(lexeme[prog+3][0]=='r')
                        {
                            temp_i.source = stoi(lexeme[prog+1].substr(1));
                            temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if(lexeme[prog+3]=="SP"||lexeme[prog+3]=="Sp"||lexeme[prog+3]=="sp")
                        {
                            temp_i.source = 13;
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instruction_file.push_back(temp_i);
                    prog+=6;
                }
                else if(lexeme[prog]=="Sub"||lexeme[prog]=="sub"||lexeme[prog]=="SUB")      //Subtract
                {
                    Instruction temp_i;
                    temp_i.opcode = 2;

                    //Sub rd,rop1,rop2
                    if(lexeme[prog+5][0]=='r')
                    {
                        temp_i.source = stoi(lexeme[prog+1].substr(1));
                        temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                        temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                        temp_i.type = 1;
                    }
                    else
                    {

                        //Add rd ,rop1,#Imm
                        if(lexeme[prog+3][0]=='r')
                        {
                            temp_i.source = stoi(lexeme[prog+1].substr(1));
                            temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                            temp_i.type = 2;
                        }
                            //Add SP,SP,#IMM
                        else if(lexeme[prog+3]=="SP"||lexeme[prog+3]=="Sp"||lexeme[prog+3]=="sp")
                        {
                            temp_i.source = 13;
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                            temp_i.type = 2;
                        }

                    }
                    instruction_file.push_back(temp_i);
                    prog+=6;
                }
                else if(lexeme[prog]=="Mul"||lexeme[prog]=="mul"||lexeme[prog]=="MUL")      //Multiply
                {
                    Instruction temp_i;
                    temp_i.opcode = 3;
                    temp_i.source = stoi(lexeme[prog+1].substr(1));
                    temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                    //Mul rd,rop1,rop2
                    if(lexeme[prog+5][0]=='r')
                    {
                        temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                        temp_i.type = 1;
                    }
                        //Mul rd, rop1,#Imm
                    else
                    {
                        temp_i.operand2 = stoi(lexeme[prog+5].substr(1));
                        temp_i.type = 2;
                    }
                    instruction_file.push_back(temp_i);
                    prog+=6;
                }
                else if(lexeme[prog]=="Mov"||lexeme[prog]=="mov"||lexeme[prog]=="MOV")      //Move
                {
                    Instruction temp_i;

                    temp_i.opcode = 4;
                    if((lexeme[prog+1] == "pc"||lexeme[prog+1] == "Pc"||lexeme[prog+1] == "PC") && (lexeme[prog+3] == "lr" || lexeme[prog+3] == "Lr" || lexeme[prog+3] == "LR"))
                    {
                        temp_i.source = -1;
                        temp_i.operand1 = 14;
                        temp_i.operand1 = -1;
                        temp_i.type = 3;
                        instruction_file.push_back(temp_i);
                    }
                    else
                    {   temp_i.source = stoi(lexeme[prog+1].substr(1));
                        temp_i.operand2 = -1;
                        if(lexeme[prog+3][0]=='r')  //Mov rd rop1
                        {
                            temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                            temp_i.type = 1;
                        }
                        else//Mov rd #12
                        {
                            temp_i.operand1 = stoi(lexeme[prog+3].substr(1));
                            temp_i.type = 2;
                        }
                        instruction_file.push_back(temp_i);
                    }
                    prog+=4;

                }
                else if(lexeme[prog]=="Ldr"||lexeme[prog]=="ldr"||lexeme[prog]=="LDR")          //Load
                {
                    //cout<<"ramlal \n";
                    Instruction temp_i;
                    temp_i.opcode = 5;
                    if(lexeme[prog+1] == "LR"||lexeme[prog+1] == "Lr"||lexeme[prog+1] == "lr")
                    {
                        temp_i.source = 14;
                    }
                    else
                    {
                        temp_i.source = stoi(lexeme[prog+1].substr(1));
                    }

                    if(lexeme[prog+3][0]=='[')      //Ldr rd, [rop1]
                    {
                        if(lexeme[prog+5][0] == ']')
                        {
                            //ldr rd,[sp]
                            if(lexeme[prog+4] == "SP"|| lexeme[prog+4] == "Sp"||lexeme[prog+4] == "sp")
                            {
                                temp_i.operand1 = 13;
                                temp_i.operand2 = -1;
                                temp_i.type=1;
                                prog+=6;
                            }
                            else
                            {
                                temp_i.operand1 = stoi(lexeme[prog+4].substr(1));
                                temp_i.operand2 = -1;
                                temp_i.type = 1;
                                prog+=6;
                            }
                        }
                        else
                        {
                            //ldr rd,[sp,#IMM]
                            if(lexeme[prog+4] == "SP"||lexeme[prog+4] == "Sp"||lexeme[prog+4] == "sp")
                            {
                                temp_i.operand1 = 13;
                                temp_i.operand2 = stoi(lexeme[prog+6].substr(1));
                                temp_i.type = 2;
                                prog+=8;
                            }
                            else
                            {
                                temp_i.operand1 = stoi(lexeme[prog+4].substr(1));
                                temp_i.operand2 = stoi(lexeme[prog+6].substr(1));
                                temp_i.type = 2;
                                prog+=8;
                            }
                        }
                    }
                    else if (lexeme[prog+3][0] == '=')      //Ldr rd, =AA
                    {
                        temp_i.operand1 = -2;
                        symb tem;                                       //change
                        tem.name = lexeme[prog + 3].substr(1);
                        tem.pos = instruction_file.size();
                        tem.type = 1;
                        temp_symb.push_back(tem);
                        temp_i.operand2 = -1;
                        temp_i.type = 3;
                        prog+=4;                                                    //change
                    }
                    instruction_file.push_back(temp_i);
                }
                else        //Store
                {
                    Instruction temp_i;
                    temp_i.opcode = 6;

                    if(lexeme[prog+1] == "LR"||lexeme[prog+1] == "Lr"||lexeme[prog+1] == "lr")
                    {
                        temp_i.source = 14;
                    }
                    else
                    {
                        temp_i.source = stoi(lexeme[prog+1].substr(1));
                    }
                    if(lexeme[prog+4][0]=='r' && lexeme[prog+5][0] == ']')      //Str rd, [rop1]
                    {
                        if(lexeme[prog+4] == "SP"||lexeme[prog+4] == "Sp"||lexeme[prog+4] == "sp")
                        {
                            temp_i.operand1 = 13;
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            prog+=6;
                        }
                        else
                        {
                            temp_i.operand1 = stoi(lexeme[prog+4].substr(1));
                            temp_i.operand2 = -1;
                            temp_i.type = 1;
                            prog+=6;
                        }
                    }
                    else        //Str rd, [rop1, #21]
                    {
                        if(lexeme[prog+4] == "SP"||lexeme[prog+4] == "Sp"||lexeme[prog+4] == "sp")
                        {
                            temp_i.operand1 = 13;
                            temp_i.operand2 = stoi(lexeme[prog+6].substr(1));
                            temp_i.type = 2;
                            prog+=8;
                        }
                        else
                        {
                            temp_i.operand1 = stoi(lexeme[prog+4].substr(1));
                            temp_i.operand2 = stoi(lexeme[prog+6].substr(1));
                            temp_i.type = 2;
                            prog+=8;
                        }
                    }
                    instruction_file.push_back(temp_i);
                }
            }

        }
        else if(lexeme[prog]=="Cmp"||lexeme[prog]=="cmp" ||lexeme[prog]=="CMP")         //compare
        {
            int q1 =prog;
            int q=compare(prog);
            prog = q1;
            if(q==-1)
            {
                cout<<"Error in cmp instruction"<<endl;
                prog++;
                errorHandler++;
            }
            else if(lexeme[prog+4] == "Bne" ||lexeme[prog+4] == "bne" ||lexeme[prog+4]=="BNE")      //cmp and bne
            {
                Instruction temp_i;
                temp_i.opcode = 7;
                temp_i.source = -2;                             //change
                symb tem;                                   //change
                tem.name = lexeme[prog + 5];
                tem.pos = instruction_file.size();
                temp_symb.push_back(tem);
                if(lexeme[prog+1][0]=='r' && lexeme[prog+3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog+1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog+3].substr(1));                                   //change
                    temp_i.type = 1;
                    prog+=6;
                }
                else if (lexeme[prog+1][0]=='r' && lexeme[prog+3][0] == '#')        //cmp rd, #5
                {
                    temp_i.operand1 = stoi(lexeme[prog+1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog+3].substr(1));                                   //change
                    temp_i.type = 2;
                    prog+=6;
                }
                instruction_file.push_back(temp_i);
            }
            else if(lexeme[prog+4] == "Bge"||lexeme[prog+4] == "bge" ||lexeme[prog+4] == "BGE")         //cmp and bge
            {
                Instruction temp_i;
                temp_i.opcode = 8;
                temp_i.source = -2;                                                             //change
                symb tem;                                                                       //change
                tem.name = lexeme[prog + 5];
                tem.pos = instruction_file.size();
                temp_symb.push_back(tem);
                if(lexeme[prog+1][0]=='r' && lexeme[prog+3][0] == 'r')          //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog+1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog+3].substr(1));
                    temp_i.type = 1;
                    prog+=6;
                }
                else if (lexeme[prog+1][0]=='r' && lexeme[prog+3][0] == '#')        //cmp rd, rop
                {
                    temp_i.operand1 = stoi(lexeme[prog+1].substr(1));                                   //change
                    temp_i.operand2 = stoi(lexeme[prog+3].substr(1));
                    temp_i.type = 2;
                    prog+=6;
                }
                instruction_file.push_back(temp_i);
            }

        }
        else if (lexeme[prog] == "BL" || lexeme[prog] == "bl" || lexeme[prog] == "Bl" )     //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = 9;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symb tem;                                                                   //change
            tem.name = lexeme[prog + 1];
            tem.pos = instruction_file.size();
            temp_symb.push_back(tem);
            prog+=2;
            instruction_file.push_back(temp_i);
        }
        else if (lexeme[prog] == "B" || lexeme[prog] == "b")        //branch
        {
            //cout<<"shyamlal"<<endl;
            Instruction temp_i;
            temp_i.opcode = 10;
            temp_i.source = -2;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = 1;
            symb tem;                                                                   //change
            tem.name = lexeme[prog + 1];
            tem.pos = instruction_file.size();
            temp_symb.push_back(tem);
            prog+=2;
            instruction_file.push_back(temp_i);
        }
        else if(lexeme[prog] == ":")
        {
            symb tem;
            tem.name = lexeme[prog-1];
            if(lexeme[prog+1] == ".space")
            {
                tem.pos = freemem;
                freemem = freemem + stoi(lexeme[prog+2]);
                tem.type = 2;
            }
            else
            {
                tem.pos = instruction_file.size();
            }
            symbol_table.push_back(tem);
            prog++;
        }
        else if(lexeme[prog] == "Exit" || lexeme[prog] == "exit" || lexeme[prog] == "EXIT" || lexeme[prog] == "Swi_Exit" || lexeme[prog] == "swi_exit" || lexeme[prog] == "SWI_EXIT")
        {
            Instruction temp_i;
            temp_i.opcode = 11;
            temp_i.source = -1;
            temp_i.operand1 = -1;
            temp_i.operand2 = -1;
            temp_i.type = -1;
            instruction_file.push_back(temp_i);
            //symb tem;
            prog++;
        }
        else prog++;
    }
    symb tem_e;
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
}
////############################END INSTRUCTION FILE#################################################


//#######################PIPELINE COMPONENTS################################
void flush();

int PCSrc = 0;
int instruction_total = 0;


//###########PIPELINE REGISTERS#############
struct IF_ID
{
    Instruction recent_instruction ;
    int prog_cnt = -1;
};

struct ID_EX
{
    int src;
    int r1;
    int r2;
    int address;
    int offset;
    int prog_cnt;
    //Controll
    int opcode = -1;
    int type;
};

struct EX_MEM
{
    int val_arith;
    int val_address;
    int src;
    bool zero;
    int prog_cnt;
    //Controll
    int opcode = -1;
    int type;
};

struct MEM_WB
{
    int write_data;
    int src;
    int prog_cnt;
    //Controll
    int opcode = -1;
    int type;
};

IF_ID Register_IF_ID;
ID_EX Register_ID_EX;
EX_MEM Register_EX_MEM;
MEM_WB Register_MEM_WB;
//****************************************

//####################PIPELINE STAGES######################
int termin;
void IF()
{
    //cout<<"IF\n";
    if(PCSrc == 0)
    {   prog++;
        if(prog >= instruction_file.size())
        {
            //cout<<"y0\n";
            Instruction end_file;
            end_file.opcode = 12;
            end_file.source = -1;
            end_file.operand1 = -1;
            end_file.operand2 = -1;
            end_file.type = -1;
            Register_IF_ID.recent_instruction = end_file;
            Register_IF_ID.prog_cnt = prog;
        }
        else if((Register_IF_ID.recent_instruction.source == instruction_file[prog].operand1 ||Register_IF_ID.recent_instruction.source == instruction_file[prog].operand2) && Register_IF_ID.recent_instruction.opcode == 5)
        {
            cout<<"stall"<<endl;
            prog--;
            Instruction bubble;
            bubble.opcode = 13;
            bubble.source = -1;
            bubble.operand1 = -1;
            bubble.operand2 = -1;
            bubble.type = -1;
            Register_IF_ID.recent_instruction = bubble;
            Register_IF_ID.prog_cnt = prog;
        }
        else
        {
            //cout<<"y\n";
            Register_IF_ID.recent_instruction = instruction_file[prog];
            Register_IF_ID.prog_cnt = prog;

            if(instruction_file[prog].opcode==1)   cout<<"Add"<<endl;
            else if(instruction_file[prog].opcode==2)  cout<<"Sub"<<endl;
            else if(instruction_file[prog].opcode==3)  cout<<"Mul"<<endl;
            else if(instruction_file[prog].opcode==4)  cout<<"Mov"<<endl;
            else if(instruction_file[prog].opcode==5)  cout<<"Ldr"<<endl;
            else if(instruction_file[prog].opcode==6)  cout<<"Str"<<endl;
            else if(instruction_file[prog].opcode==7)  cout<<"bne & cmp"<<endl;
            else if(instruction_file[prog].opcode==8)  cout<<"bge & cmp"<<endl;
            else if(instruction_file[prog].opcode==9)  cout<<"Bl"<<endl;
            else if(instruction_file[prog].opcode==10)  cout<<"branch"<<endl;
            else if(instruction_file[prog].opcode==11)   cout<<"exit"<<endl;
            else if(instruction_file[prog].opcode==12)   cout<<"endOfFile"<<endl;
        }
    }
    else
    {
        //cout<<"y00\n";
        flush();
        PCSrc = 0;
    }


}

void ID()
{
    //cout<<"ID\n";
    Register_ID_EX.prog_cnt = Register_IF_ID.prog_cnt;
    if(Register_IF_ID.recent_instruction.opcode <=3 && Register_IF_ID.recent_instruction.opcode > 0)
    {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard_r1 = 0;
        bool hazard_r2 = 0;

        if(Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode < 6)
        {
            //cout<<"h "<< Register_IF_ID.recent_instruction.operand1<<endl;
            //cout<<Register_MEM_WB.write_data<<endl;
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
            hazard_r1 = 1;
        }
        else if(Register_IF_ID.recent_instruction.operand2 == Register_MEM_WB.src && Register_IF_ID.recent_instruction.type == 1  && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode < 6)
        {
            //cout<<"hr"<<endl;
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_MEM_WB.write_data;
        }

        if(Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode < 6)
        {
            //cout<<"hfe"<<endl;
            cout<<Register_EX_MEM.val_arith<<endl;
            hazard_r1 = 1;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }
        if(Register_IF_ID.recent_instruction.operand2 == Register_EX_MEM.src && Register_IF_ID.recent_instruction.type == 1  && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode < 6)
        {
            //cout<<"hewf"<<endl;
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }



        if(hazard_r1 == 0)
        {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        }

        if(Register_IF_ID.recent_instruction.type == 1 && hazard_r2 == 0)
        {
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
        }
        else if(Register_IF_ID.recent_instruction.type == 2)
        {
            Register_ID_EX.r2 = Register_IF_ID.recent_instruction.operand2;
        }
    }
    else if(Register_IF_ID.recent_instruction.opcode == 4)
    {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard = 0;

        if(Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src && Register_IF_ID.recent_instruction.type == 1 && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            hazard = 1;
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
        }
        if(Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src && Register_IF_ID.recent_instruction.type == 1 && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6)
        {
            hazard = 1;
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }

        if(Register_IF_ID.recent_instruction.type == 1  && hazard == 0)
        {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        }
        else if(Register_IF_ID.recent_instruction.type == 2 && hazard == 0)
        {
            Register_ID_EX.r1 = Register_IF_ID.recent_instruction.operand1;
        }
        else if(Register_IF_ID.recent_instruction.type == 3)
        {

            if(Register_MEM_WB.src  == 14)
            {
                Register_ID_EX.r1 = Register_MEM_WB.write_data;
            }
            else if(14 == Register_EX_MEM.src)
            {
                Register_ID_EX.r1 = Register_EX_MEM.val_arith;
            }
            else
                Register_ID_EX.r1 = r[14];
        }
    }
    else if(Register_IF_ID.recent_instruction.opcode == 5 )
    {
        Register_ID_EX.src = Register_IF_ID.recent_instruction.source;
        bool hazard = 0;
        bool hazard_s = 0;
        if(Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            hazard = 1;
            Register_ID_EX.address = Register_MEM_WB.write_data;
        }
        if(Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6)
        {
            hazard = 1;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if(Register_IF_ID.recent_instruction.type == 1 && hazard == 0)
        {
            Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
        }
        else if(Register_IF_ID.recent_instruction.type == 2)
        {
            if(hazard == 0)
            {   Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
            }
            Register_ID_EX.offset = Register_IF_ID.recent_instruction.operand2;
        }
        else if(Register_IF_ID.recent_instruction.type == 3)
        {
            Register_ID_EX.address = Register_IF_ID.recent_instruction.operand1;
        }
    }
    else if(Register_IF_ID.recent_instruction.opcode == 6)
    {
        Register_ID_EX.src = r[Register_IF_ID.recent_instruction.source];
        bool hazard = 0;
        bool hazard_s = 0;

        if(Register_IF_ID.recent_instruction.source == Register_MEM_WB.src && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            //cout<<"k"<<endl;
            hazard = 1;
            Register_ID_EX.src = Register_MEM_WB.write_data;
        }
        if(Register_IF_ID.recent_instruction.source == Register_EX_MEM.src && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6 )
        {
            //cout<<"jf"<<endl;
            hazard = 1;
            Register_ID_EX.src = Register_EX_MEM.val_arith;
        }

        if(Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            //cout<<"kbd"<<endl;
            hazard = 1;
            Register_ID_EX.address = Register_MEM_WB.write_data;
        }
        if(Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6)
        {
            //cout<<"kbdk"<<endl;
            hazard = 1;
            Register_ID_EX.address = Register_EX_MEM.val_arith;
        }

        if(Register_IF_ID.recent_instruction.type == 1 && hazard == 0)
        {
            Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
        }
        else if(Register_IF_ID.recent_instruction.type == 2)
        {
            if(hazard == 0)
            {   Register_ID_EX.address = r[Register_IF_ID.recent_instruction.operand1];
            }
            Register_ID_EX.offset = Register_IF_ID.recent_instruction.operand2;
        }
    }
    else if(Register_IF_ID.recent_instruction.opcode > 6 && Register_IF_ID.recent_instruction.opcode < 9 )
    {
        bool hazard_r1 = 0;
        bool hazard_r2 = 0;

        if(Register_IF_ID.recent_instruction.operand1 == Register_MEM_WB.src && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            //cout<<"h"<<endl;
            Register_ID_EX.r1 = Register_MEM_WB.write_data;
            hazard_r1 = 1;
        }
        else if(Register_IF_ID.recent_instruction.operand2 == Register_MEM_WB.src && Register_IF_ID.recent_instruction.type == 1  && Register_MEM_WB.opcode>0 && Register_MEM_WB.opcode <= 6)
        {
            //cout<<"hd"<<endl;
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_MEM_WB.write_data;
        }
        if(Register_IF_ID.recent_instruction.operand1 == Register_EX_MEM.src && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6 )
        {
            //cout<<"hre"<<endl;
            hazard_r1 = 1;
            Register_ID_EX.r1 = Register_EX_MEM.val_arith;
        }
        if(Register_IF_ID.recent_instruction.operand2 == Register_EX_MEM.src && Register_IF_ID.recent_instruction.type == 1 && Register_EX_MEM.opcode>0 && Register_EX_MEM.opcode <= 6)
        {
            //cout<<"hkjd"<<endl;
            hazard_r2 = 1;
            Register_ID_EX.r2 = Register_EX_MEM.val_arith;
        }



        if(hazard_r1 == 0)
        {
            Register_ID_EX.r1 = r[Register_IF_ID.recent_instruction.operand1];
        }

        if(Register_IF_ID.recent_instruction.type == 1 && hazard_r2 == 0)
        {
            Register_ID_EX.r2 = r[Register_IF_ID.recent_instruction.operand2];
        }
        else if(Register_IF_ID.recent_instruction.type == 2)
        {
            Register_ID_EX.r2 = Register_IF_ID.recent_instruction.operand2;
        }

        Register_ID_EX.address = Register_IF_ID.recent_instruction.source;
    }
    else if(Register_IF_ID.recent_instruction.opcode == 10 || Register_IF_ID.recent_instruction.opcode == 9)
    {
        Register_ID_EX.address = Register_IF_ID.recent_instruction.source;
    }

    Register_ID_EX.opcode = Register_IF_ID.recent_instruction.opcode;
    Register_ID_EX.type = Register_IF_ID.recent_instruction.type;

    if(Register_IF_ID.recent_instruction.opcode==1)   cout<<"Add"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==2)  cout<<"Sub"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==3)  cout<<"Mul"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==4)  cout<<"Mov"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==5)  cout<<"Ldr"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==6)  cout<<"Str"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==7)  cout<<"bne & cmp"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==8)  cout<<"bge & cmp"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==9)  cout<<"Bl"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==10)  cout<<"branch"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==11)   cout<<"exit"<<endl;
    else if(Register_IF_ID.recent_instruction.opcode==12)   cout<<"endOfFile"<<endl;
}

void EX()
{
    //cout<<"EX\n";
    Register_EX_MEM.prog_cnt = Register_ID_EX.prog_cnt;
    if( Register_ID_EX.opcode <9 && Register_ID_EX.opcode > 0)
    {
        Register_EX_MEM.src = Register_ID_EX.src;
        if(Register_ID_EX.opcode >= 1 && Register_ID_EX.opcode <= 4)
        {
            if(Register_ID_EX.opcode == 1)
            {
                Register_EX_MEM.val_arith = Register_ID_EX.r1 + Register_ID_EX.r2;
            }
            else if(Register_ID_EX.opcode == 2)
            {
                Register_EX_MEM.val_arith = Register_ID_EX.r1 - Register_ID_EX.r2;
            }
            else if(Register_ID_EX.opcode == 3)
            {
                Register_EX_MEM.val_arith = Register_ID_EX.r1 * Register_ID_EX.r2;
            }
            else if(Register_ID_EX.opcode == 4)
            {
                Register_EX_MEM.val_arith = Register_ID_EX.r1;
            }
        }
        else if(Register_ID_EX.opcode == 5 || Register_ID_EX.opcode == 6)
        {
            if (Register_ID_EX.type == 1)
            {
                Register_EX_MEM.val_address = Register_ID_EX.address;
            }
            else if (Register_ID_EX.type == 2)
            {
                Register_EX_MEM.val_address = Register_ID_EX.address + Register_ID_EX.offset;
            }
            else
            {
                Register_EX_MEM.val_address = Register_ID_EX.address;
            }
        }
        else if(Register_ID_EX.opcode > 6 && Register_ID_EX.opcode < 9)
        {
            if((Register_ID_EX.r1 - Register_ID_EX.r2) == 0 && Register_ID_EX.opcode == 7)
            {   Register_EX_MEM.zero = 1;
                //cout<<"Register_ID_EX.r1 = "<<Register_ID_EX.r1<<"\t"<<"Register_ID_EX.r2 = "<<Register_ID_EX.r2<<endl;
                //cout<<"equal"<<endl;
            }
            else if((Register_ID_EX.r1 - Register_ID_EX.r2) < 0 && Register_ID_EX.opcode == 8)
            {   Register_EX_MEM.zero = 1;
                //cout<<"Register_ID_EX.r1 = "<<Register_ID_EX.r1<<"\t"<<"Register_ID_EX.r2 = "<<Register_ID_EX.r2<<endl;
                //cout<<"equal"<<endl;
            }
            else
            {   Register_EX_MEM.zero = 0;
                //cout<<"Register_ID_EX.r1 = "<<Register_ID_EX.r1<<"\t"<<"Register_ID_EX.r2 = "<<Register_ID_EX.r2<<endl;
                //cout<<"not equal"<<endl;
            }

            Register_EX_MEM.val_address = Register_ID_EX.address;
            //cout<<Register_EX_MEM.val_address<<"gudsjeffkjbfs\n";
        }
    }
    else  if (Register_ID_EX.opcode == 10 || Register_ID_EX.opcode == 9)
    {
        Register_EX_MEM.val_address = Register_ID_EX.address;
        Register_EX_MEM.zero = 1;
    }
    Register_EX_MEM.opcode = Register_ID_EX.opcode;
    Register_EX_MEM.type = Register_ID_EX.type;

    if(Register_ID_EX.opcode==1)   cout<<"Add"<<endl;
    else if(Register_ID_EX.opcode==2)  cout<<"Sub"<<endl;
    else if(Register_ID_EX.opcode==3)  cout<<"Mul"<<endl;
    else if(Register_ID_EX.opcode==4)  cout<<"Mov"<<endl;
    else if(Register_ID_EX.opcode==5)  cout<<"Ldr"<<endl;
    else if(Register_ID_EX.opcode==6)  cout<<"Str"<<endl;
    else if(Register_ID_EX.opcode==7)  cout<<"bne & cmp"<<endl;
    else if(Register_ID_EX.opcode==8)  cout<<"bge & cmp"<<endl;
    else if(Register_ID_EX.opcode==9)  cout<<"Bl"<<endl;
    else if(Register_ID_EX.opcode==10)  cout<<"branch"<<endl;
    else if(Register_ID_EX.opcode==11)   cout<<"exit"<<endl;
    else if(Register_ID_EX.opcode==12)   cout<<"endOfFile"<<endl;
}

void MEM()
{
    //cout<<"MEM\n";
    Register_MEM_WB.prog_cnt = Register_EX_MEM.prog_cnt;
    if(Register_EX_MEM.opcode < 5 && Register_EX_MEM.opcode > 0)
    {
        Register_MEM_WB.write_data = Register_EX_MEM.val_arith;
        Register_MEM_WB.src = Register_EX_MEM.src;
    }
    else if(Register_EX_MEM.opcode == 5)
    {
        if(Register_EX_MEM.type != 3)
        {   Register_MEM_WB.write_data = mem[(Register_EX_MEM.val_address)/4];
            cout<<"mem["<<(Register_EX_MEM.val_address)/4<<"]="<<mem[(Register_EX_MEM.val_address)/4]<<endl;
            Register_MEM_WB.src = Register_EX_MEM.src;
        }
        else
        {
            Register_MEM_WB.write_data = (Register_EX_MEM.val_address);
            Register_MEM_WB.src = Register_EX_MEM.src;
        }
    }
    else if(Register_EX_MEM.opcode == 6)
    {
        mem[(Register_EX_MEM.val_address)/4] = Register_EX_MEM.src;
        cout<<"mem["<<Register_EX_MEM.val_address/4<<"] = "<<Register_EX_MEM.src<<"\n";
    }

    if(Register_EX_MEM.opcode > 6 && Register_EX_MEM.opcode < 9 && Register_EX_MEM.zero == 0)
    {
        //cout<<"PCSrc = "<<PCSrc<<endl;
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }

    if(Register_EX_MEM.opcode == 9)
    {
        r[14] = Register_EX_MEM.prog_cnt + 1;
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }

    if(Register_EX_MEM.opcode == 4 && Register_EX_MEM.type == 3)
    {
        //cout<<"jhevwkjdbbndkjlnkljbnkjbkndka\n";
        Register_EX_MEM.val_address = Register_EX_MEM.val_arith;
        PCSrc = 1;
    }

    if(Register_EX_MEM.opcode == 10)
    {
        PCSrc = 1;
        Register_EX_MEM.zero == 0;
    }
    Register_MEM_WB.opcode = Register_EX_MEM.opcode;
    Register_MEM_WB.type = Register_EX_MEM.type;

    if(Register_EX_MEM.opcode==1)   cout<<"Add"<<endl;
    else if(Register_EX_MEM.opcode==2)  cout<<"Sub"<<endl;
    else if(Register_EX_MEM.opcode==3)  cout<<"Mul"<<endl;
    else if(Register_EX_MEM.opcode==4)  cout<<"Mov"<<endl;
    else if(Register_EX_MEM.opcode==5)  cout<<"Ldr"<<endl;
    else if(Register_EX_MEM.opcode==6)  cout<<"Str"<<endl;
    else if(Register_EX_MEM.opcode==7)  cout<<"bne & cmp"<<endl;
    else if(Register_EX_MEM.opcode==8)  cout<<"bge & cmp"<<endl;
    else if(Register_EX_MEM.opcode==9)  cout<<"Bl"<<endl;
    else if(Register_EX_MEM.opcode==10)  cout<<"branch"<<endl;
    else if(Register_EX_MEM.opcode==11)   cout<<"exit"<<endl;
    else if(Register_EX_MEM.opcode==12)   cout<<"endOfFile"<<endl;
}

void WB()
{
    //cout<<"WB\n";
    if(Register_MEM_WB.opcode <= 5 && Register_MEM_WB.opcode > 0)
    {
        r[Register_MEM_WB.src] = Register_MEM_WB.write_data;
    }

    else if (Register_MEM_WB.opcode == 11)
    {
        shut_down = 1;
    }
    else if(Register_MEM_WB.opcode == 12)
    {
        end_file_inst = 1;
    }

    if(Register_MEM_WB.opcode==1)   cout<<"Add"<<endl;
    else if(Register_MEM_WB.opcode==2)  cout<<"Sub"<<endl;
    else if(Register_MEM_WB.opcode==3)  cout<<"Mul"<<endl;
    else if(Register_MEM_WB.opcode==4)  cout<<"Mov"<<endl;
    else if(Register_MEM_WB.opcode==5)  cout<<"Ldr"<<endl;
    else if(Register_MEM_WB.opcode==6)  cout<<"Str"<<endl;
    else if(Register_MEM_WB.opcode==7)  cout<<"bne & cmp"<<endl;
    else if(Register_MEM_WB.opcode==8)  cout<<"bge & cmp"<<endl;
    else if(Register_MEM_WB.opcode==9)  cout<<"Bl"<<endl;
    else if(Register_MEM_WB.opcode==10)  cout<<"branch"<<endl;
    else if(Register_MEM_WB.opcode==11)   cout<<"exit"<<endl;
    else if(Register_MEM_WB.opcode==12)   cout<<"endOfFile"<<endl;
}

void flush()
{
    prog = Register_EX_MEM.val_address-1;
    //cout<<"flush\n";
    //cout<<"Register_EX_MEM.val_address = "<<Register_EX_MEM.val_address<<endl;
    Register_ID_EX.opcode = -1;
    Register_ID_EX.type = -1;
    Register_EX_MEM.opcode = -1;
    Register_EX_MEM.type = -1;
    Register_IF_ID.recent_instruction.opcode = -1;
    Register_IF_ID.recent_instruction.type = -1;

}
//****************************************PIPELINE STAGES END*************************


//#####################LATENCY ANALYSIS#########################
int latency_data[12];
int latency_total = 0;

string priority[13] = { "add", "sub", "mul", "mov", "ldr", "ldr_pseudo", "str", "cmp", "bne", "bge", "b", "bl"};


void read_latency()
{
    fin.open("./latency.txt");
    char s;
    char *t;
    int capacity = 10;
    t = new char[capacity];
    int ssize = 0;
    int k =1;
    vector<string> temps;
    bool comment = 0 ;

    while (!fin.eof())
    {


        fin.get(s);

        if(int(s)==10)
        { comment = 0;
        }
        else
        if(int(s) == 35)
            comment = 1;

        if (comment ==0)
        {
            if(!(s<123&&int(s)>96||int(s)>64&&int(s)<91||int(s)>=48&&int(s)<=57||int(s)==95))
            {
                t[ssize]='\0';
                if(ssize>=1)
                {
                    temps.push_back(t);
                }
                ssize= 0;
                capacity=10;
                t=new char [capacity];

            }

            else
            {
                if(ssize<capacity)
                {    t[ssize]=s;
                    ssize++;
                }
                else
                {   char*temp = t;

                    t = new char[capacity*2];
                    capacity = 2*capacity;
                    for(int i=0; i<ssize;i++)
                        t[i] = temp[i];
                    t[ssize] = s;
                    ssize++;
                    delete []temp;
                }
            }
        }
    }

    fin.close();
    for(int i=0;i<temps.size();i++)
    {
        //cout<<temps[i]<<endl;
        for(int j=0; j<13;j++)
            if(temps[i] == priority[j])
            {
                latency_data[j] = stoi(temps[i+1]);
                i++;
            }
    }


}

int latency_fun()
{
    int latency_pipe[5];
    latency_pipe[0] = 1;
    latency_pipe[1] = 1;
    if(Register_ID_EX.opcode > 0 && Register_ID_EX.opcode <= 4)
    {
        latency_pipe[2] = latency_data[Register_ID_EX.opcode - 1];
    }
    else if(Register_ID_EX.opcode == 7 || Register_ID_EX.opcode == 8)
    {
        latency_pipe[2] = latency_data[7];
    }
    else if(Register_ID_EX.opcode == 5 && Register_ID_EX.type == 3)
    {
        latency_pipe[2] = latency_data[5];
    }
    else
    {
        latency_pipe[2] = 1;
    }

    if(Register_EX_MEM.opcode == 5)
    {
        latency_pipe[3] = latency_data[4];
    }
    else if(Register_EX_MEM.opcode == 6)
    {
        latency_pipe[3] = latency_data[6];
    }
    else if(Register_EX_MEM.opcode == 7 || Register_EX_MEM.opcode == 8)
    {
        latency_pipe[3] = latency_data[Register_EX_MEM.opcode +1];
    }
    else if(Register_EX_MEM.opcode == 10)
    {
        latency_pipe[3] = latency_data[10];
    }
    else if(Register_EX_MEM.opcode == 9)
    {
        latency_pipe[3] = latency_data[11];
    }
    else
    {
        latency_pipe[3] = 1;
    }

    latency_pipe[4] = 1;

    int max = 1;
    for (int i = 0; i < 5; ++i)
    {
        if(max < latency_pipe[i])
            max = latency_pipe[i];
    }
    return max;
}
//**********************************LATENCY ANALYSIS END ********************************

int main()
{

    char s;
    char *t;
    int capacity = 10;
    t = new char[capacity];
    int ssize = 0;
    int k =1;
    vector<string> temps;
    //int k =1;
    int sum = 0;
    int pmax=0;
    bool comment = 0 ;
    bool loop_or_data = 0;
    fin.open("./input.txt");
    while (!fin.eof())
    {
        fin.get(s);
        {
            if (int(s)==64)
            {
                if(comment == 0)
                    comment = 1;
            }
            else
            if (int(s)==10 && comment == 1)
            {
                comment= 0;
            }

            if(comment == 0)
            {
                if(!(s<123&&int(s)>96||int(s)==39||int(s)>64&&int(s)<91||int(s)==46||int(s)==95||int(s)>47&&int(s)<58||int(s)>39&&int(s)<43||int(s)==35||int(s)==61))
                {
                    if(int(s) == 44 || int(s) == 58|| int(s) == 59|| int(s)==91 || int(s)==93)
                    {
                        if(ssize>=2)
                        {
                            t[ssize]='\0';
                            lexeme.push_back(t);


                            if(int(s) == 44)
                                lexeme.push_back(",");
                            else if(int(s) == 58)
                            {

                                if(loop_or_data == 0)
                                {
                                    loop temp;
                                    temp.loop_name =  lexeme[lexeme.size()-1];
                                    temp.loop_no = lexeme.size()-1;
                                    loops.push_back(temp);
                                }
                                else
                                {
                                    loop1 temp;
                                    temp.loop_name =  lexeme[lexeme.size()-1];
                                    temp.loop_no = lexeme.size()-1;
                                    temp.start = 0;
                                    datas.push_back(temp);

                                }
                                lexeme.push_back(":");
                            }
                            else if(int(s) == 59)
                                lexeme.push_back(";");
                            else if(int(s) == 91)
                                lexeme.push_back("[");
                            else if(int(s) == 93)
                                lexeme.push_back("]");

                        }
                        else
                        {
                            if(int(s) == 44)
                                lexeme.push_back(",");
                            if(int(s) == 58)
                            {
                                loop temp;
                                temp.loop_name =  lexeme[lexeme.size()-1];
                                temp.loop_no = lexeme.size()-1;
                                if(loop_or_data == 0)
                                    loops.push_back(temp);
                                else
                                {
                                    loop1 temp;
                                    temp.loop_name =  lexeme[lexeme.size()-1];
                                    temp.loop_no = lexeme.size()-1;
                                    temp.start = -1;
                                    datas.push_back(temp);

                                }

                                lexeme.push_back(":");
                            }
                            if(int(s) == 59)
                                lexeme.push_back(";");
                            else if(int(s) == 91)
                                lexeme.push_back("[");
                            else if(int(s) == 93)
                                lexeme.push_back("]");
                        }
                    }
                    else
                    {
                        t[ssize]='\0';
                        if(ssize>=1)
                        {
                            lexeme.push_back(t);

                        }
                        if(lexeme.size()>0)
                        {
                            if( lexeme[lexeme.size() - 1] == ".data")
                            {//cout<<"DATA"<<endl;
                                loop_or_data = 1;}
                            else
                            if(lexeme[lexeme.size() - 1] == ".text")
                            {loop_or_data = 0;}
                                // cout<<"TEXT"<<endl;}
                            else if(lexeme[lexeme.size()-1]=="0x11")
                                exit_name = lexeme[lexeme.size()-3];
                        }

                    }
                    sum = 0;
                    ssize= 0;
                    capacity=10;
                    t=new char [capacity];
                }

                else
                {
                    if(ssize<capacity)
                    {
                        t[ssize]=s;
                        ssize++;
                    }
                    else
                    {
                        char*temp = t;
                        t = new char[capacity*2];
                        capacity = 2*capacity;
                        for(int i=0; i<ssize;i++)
                            t[i] = temp[i];
                        t[ssize] = s;
                        ssize++;
                        delete []temp;
                    }
                }
            }
        }
    }
    fin.close();

    //driver statement
    make_instruction_file();
    read_latency();


//symbol table
    for (int i = 0; i < symbol_table.size(); ++i)
    {
        for (int j = 0; j < temp_symb.size(); ++j)
        {
            if(symbol_table[i].name == temp_symb[j].name)
            {
                if(temp_symb[j].type == 0)
                {   instruction_file[temp_symb[j].pos].source = symbol_table[i].pos;
                }
                else
                {
                    instruction_file[temp_symb[j].pos].operand1 = symbol_table[i].pos;
                }
            }
        }
    }
    prog = -1;
    if(errorHandler==0)
    {
        while(shut_down == 0 && end_file_inst == 0)
        {
            if(Register_MEM_WB.opcode >=1 && Register_MEM_WB.opcode <= 10 )
                instruction_total++;
            cout<<BOLD(FGRN("WB"))<<"\n";
            WB();
            for (int i = 0; i < 16; ++i)
            {
                cout<<"r["<<i<<"]="<<r[i]<<"\t";
            }
            cout<<"\n";

            //GUI
            cout<<BOLD(FRED("MEM"))<<endl;
            MEM();

            if(PCSrc == 0)
            {
                cout<<BOLD(FYEL("EX"))<<endl;
                EX();

                cout<<BOLD(FMAG("ID"))<<endl;
                ID();


            }
            else
            {
                cout<<BOLD(FYEL("EX"))<<endl;
                cout<<"flush"<<endl;
                cout<<BOLD(FMAG("ID"))<<endl;
                cout<<"flush"<<endl;
            }
            cout<<BOLD(FCYN("IF"))<<endl;
            IF();

            latency_total = latency_total + 5*latency_fun();
            cout<<UNDL(FCYN("latency_total = "))<<latency_total<<endl<<endl;
        }
        cout<<"Total Instructions are "<<instruction_total<<" & Total Latency is "<<latency_total<<", So The CPI will be nearly "<<float(latency_total)/float(instruction_total)<<endl;
    }
}
