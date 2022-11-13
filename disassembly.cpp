#include<cstdio>
#include<algorithm>
#include<iostream>
#include<cmath>
#include<string.h>
#include<fstream>
#include<vector>

#include<memory.h>
#include <map>                                              //store memory data
// #include<disassmble.h>

using namespace std;




struct Inst{
    string Itpye = "";
    string op = "";
    string instr_b = "";
    string assmble = "";
    int address = 0;
    int rs = 0;
    int rt = 0;
    int rd = 0;
    int immd = 0;
    int shamt = 0;
    int funct = 0;
};
//Inst* inst = nullptr;

int read_file(char* file);                          //read binary code

Inst* Decode_inst(string cur_inst);               //instruction decode
int string_32b_To_int(string input);             //32bit string change to unsigned int
void Decode();
void Excute();
void Init();
//void Inst_print(Inst* inst);
void Inst_print(Inst* inst, ofstream &outFile);
void Register_print();
void Data_print();




int count = 0;                                      ///code line 
int PC = 0;                                         //program count
int cycle = 0;                                      //programe cycle
string line;  
string first_bit;          
string op, rs, rt, rd, shamt, funct, imm;
vector<string> bin_set;                              // binart set
//Inst  instruction ;
//inst = &instruction;
vector<Inst*> Inst_set;                             // instrction set

ofstream outFile;	                                //define ofstream object outFile
ofstream outFile_simu;
int Register[32];
map<int,int> mem_map;
int mem_start,mem_end;                              //record the start and end of the memory data


int main(int argc, char* argv[])
{
	//instruction  = new Inst();
    outFile.open("disassmbly.txt");
    outFile_simu.open("simulation.txt");
    int return_code = 1;

    Init();
    return_code = read_file(argv[1]);
    
    if(return_code == 0)
        return 0;

    Decode();

    
    // for(int i = 0; i < Inst_set.size(); i++)
    //     cout<<i<<"   "<<Inst_set[i]->op<<endl;

    Excute();

    cout<<mem_start<<"   "<<mem_end<<endl;

    outFile.close();
    outFile_simu.close();
	return 0;
}

void Init()
{

    memset(Register, 0, sizeof(int)*32);  // init the 32 Register
}



int read_file(char* file)
{
    ifstream in(file);   //读取文件
    string filename;        
    int state;              
    int count = 0;
    if(in) // 有该文件
    {
        while (getline (in, line)) // line中不包括每行的换行符
        { 
            bin_set.push_back(line);
            count++;
        }
        return 1;
    }
    else // 没有该文件
    {   
        
        cout <<"no such file" << endl;
        return 0;
    }
  

}


void Decode()
{

    int state = 0;
    for(PC = 0; PC < bin_set.size(); PC++)
    {
        line = bin_set[PC];
        if(state != 1)
        {
            Inst* inst1 = Decode_inst(line);
            Inst_set.push_back(inst1);
            state = inst1->op == "BREAK" ? 1 : 0;
            if(state == 1)
            {
                mem_start = 64+PC * 4 + 4;
            }
        }
        else
        {   
            line = line.substr(0, 32);                              //attention！！
            outFile<<line<<"\t";
           // cout<<line<<"\t";
            mem_map[64+PC*4] = string_32b_To_int(line);
            outFile<<64+PC*4<<"\t"<<string_32b_To_int(line)<<endl;

            //cout<<64+PC*4<<"\t"<<string_32b_To_int(line)<<endl;
        }
        
    }

    mem_end = 64 + (bin_set.size() - 1) * 4;
    for(int ii = mem_start ; ii<=mem_end;ii = ii + 4)
    {
        cout<<mem_map[ii]<<" ";
    }
  //  Data_print();
}



Inst* Decode_inst(string cur_inst)
{

    Inst*  inst = new Inst();
    first_bit = cur_inst.substr(0, 1);
    op = cur_inst.substr(1, 5);
    int operation = stoi(op, 0, 2);


    rs = cur_inst.substr(6,5);
    rt = cur_inst.substr(11,5);
    rd = cur_inst.substr(16,5);
    shamt = cur_inst.substr(21,5);
    funct = cur_inst.substr(26, 6);
    int rs_int = stoi(rs, 0, 2);
    int rt_int = stoi(rt, 0, 2);
    int rd_int = stoi(rd, 0, 2);
    int shamt_int = stoi(shamt, 0, 2);
    int function = stoi(funct, 0, 2);

    inst->rs = rs_int;
    inst->rt = rt_int;
    inst->rd = rd_int;
    inst->shamt = shamt_int;
    inst->funct = function;
    inst->address = 64 + PC * 4;
    outFile<<first_bit<<op<<" "<<rs<<" "<<rt<<" "<<rd<<" "<<shamt<<" "<<funct<<"\t";
    outFile<<inst->address<<"\t";

    //cout<<first_bit<<op<<" "<<rs<<" "<<rt<<" "<<rd<<" "<<shamt<<" "<<funct<<"\t";
    //cout<<64+PC*4<<"\t";

   //         -----------R type------------------
    if(operation == 0 || operation == 0b11100)
    {

        inst->Itpye = "R";
   
        
        
        if(operation == 0)
        {
            switch (function)
            {
            case  0b001000:
                inst->op = "JR";

                /* code */
                break;
            case  0b001101:
                inst->op = "BREAK";
                /* code */
                break;
            case  0b000000:
                if(rs_int == 0 && rt_int && 0 && rd_int == 0 && shamt_int == 0)
                {
                    inst->op = "NOP";
                }
                else
                {
                    inst->op = "SLL";
                }
                /* code */
                break;

            case  0b000010:
                
                inst->op = "SRL";
                /* code */
                break;
            case  0b000011:
                
                inst->op = "SRA";
                /* code */
                break;
            case  0b100000:
                inst->op = "ADD";
                /* code */
                break;
            case  0b0100010:
                
                inst->op = "SUB";
                /* code */
                break;
            case  0b100100:
                
                inst->op = "AND";
                /* code */
                break;

            case  0b100111:
                
                inst->op = "NOR";
                /* code */
                break;

            case  0b101010:
                
                inst->op = "SLT";
                /* code */
                break;
            default:
                break;
            }
        }
        else if (operation == 0b11100)
        {
             inst->op = "MUL";
        }
    }

    // //         -------------------J type-------------------------
    else if(operation == 0b00010)
    {
       inst->Itpye = "J";
       imm = cur_inst.substr(16,16);

        int imm_int = stoi(imm, 0, 2);

        inst->immd = imm_int;
        switch (operation)
        {

            //J型
            case 0b00010:
                /* code */
                inst->op = "J";
                break;
            
            //BEQ型  I
            case 0b00100:
                inst->op = "BEQ";

                /* code */
                break;

            //BLTZ型  I
            case 0b00001:
                /* code */
                inst->op = "BLTZ";
                break;

            //BGTZ型  I
            case 0b00111:
            inst->op = "BGTZ";
                /* code */
                break;


            default:
                break;
        }
        

    }
     //         -----------I type------------------
    else 
    {   
        inst->Itpye = "I";
   
       
        rs = cur_inst.substr(6,5);
        rt = cur_inst.substr(11,5);
        imm = cur_inst.substr(16, 16);

        int rs_int = stoi(rs, 0, 2);
        int rt_int = stoi(rt, 0, 2);
        int imme_int = stoi(imm, 0, 2);
            
        inst->rs = rs_int;
        inst->rt = rt_int;
        inst->immd = imme_int;

        if(stoi(first_bit) == 1)                    //the special imme instruction ：first bit == 1
        {
            switch (operation)
            {
                
                //MUL 
                case 0b00001:
                    /* code */
                    inst->op = "MUL";
                    break;   

                //SW 
                case 0b01011:
                    /* code */
                    inst->op = "SW";
                    break; 
                //LW 
                case 0b00011:
                    /* code */
                    inst->op = "LW";
                    break; 

                //ADD 
                case 0b10000:
                    /* code */
                    inst->op = "ADD";
                    break;
                //SUB
                case 0b10001:
                    /* code */
                    inst->op = "SUB";
                    break;
                //AND
                case 0b10010:
                    /* code */
                    inst->op = "AND";
                    break;
                //NOR
                case 0b10011:
                    /* code */
                    inst->op = "NOR";
                    break;
                //SLT
                case 0b10101:
                    /* code */
                    inst->op = "SLT";
                    break;
                default:
                    break;
            }

        }
        else                                            //the normal imme instruction 
        {
            switch (operation)
            {
                //BEQ型   I
                case 0b00100:
                    inst->op = "BEQ";

                    /* code */
                    break;

                //BLTZ型  I
                case 0b00001:
                    /* code */
                    inst->op = "BLTZ";
                    break;

                //BGTZ型  I
                case 0b00111:
                inst->op = "BGTZ";
                    /* code */
                    break;
            }
        }
    }
    Inst_print(inst, outFile);
    return inst;
}

void Excute()
{
    PC = 0;
    int branch = 0;                     //is program  jump 
    //outFile_simu<<"--------------------"<<endl;
    do
    {
        Inst* inst = Inst_set[PC];
        branch = 0;
        outFile_simu<<"--------------------"<<endl;
        outFile_simu<<"Cycle:"<<cycle + 1<<"\t"<<Inst_set[PC]->address<<"\t";
        Inst_print(Inst_set[PC], outFile_simu); 
        outFile_simu <<endl;
        
        
        if(inst->Itpye == "R")
        {
            if(inst->op == "BREAK")
            {

            }
                //outFile<<"BREAK"<<endl;

                //cout<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rs<<", R"<<inst->rt<<endl;
            else if(inst->op == "SLL"||inst->op == "SRL")
            {
                if(inst->op == "SLL")
                {
                    Register[inst->rd] = Register[inst->rt] << inst->shamt;
                }
                else if(inst->op == "SRL")
                {
                    Register[inst->rd] = Register[inst->rt] >> inst->shamt;

                }
               // outFile<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rt<<", #"<<inst->shamt<<endl;

            }
            else
            {
                if(inst->op == "ADD")
                {
                    Register[inst->rd] = Register[inst->rs] + Register[inst->rt];
                }
                else if(inst->op == "SUB")
                {
                    Register[inst->rd] = Register[inst->rs] - Register[inst->rt];

                }
                else if(inst->op == "MUL")
                {
                    Register[inst->rd] = Register[inst->rs] * Register[inst->rt];

                }
                else if(inst->op == "AND")
                {
                    Register[inst->rd] = Register[inst->rs] && Register[inst->rt];

                }
                else if(inst->op == "NOR")
                {
                    Register[inst->rd] = !(Register[inst->rs] || Register[inst->rt]);

                }
                else if(inst->op == "SLT")
                {
                    Register[inst->rd] = (Register[inst->rs] < Register[inst->rt]) ? 1 : 0;

                }
                else if(inst->op == "JR")           //JR rs
                {
                    PC = Register[inst->rs];
                    branch = 1;
                }
                else if(inst->op =="SRA")     //signed shift
                {
                    Register[inst->rd] = Register[inst->rt] >> inst->shamt;

                }
                else if(inst->op == "NOP")
                {

                }
               // outFile<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rs<<", R"<<inst->rt<<endl;
            }

     
        }
        else if(inst->Itpye == "J")
        {

            if(inst->op == "J")
            {
                PC = (inst->immd *4 - 64) / 4;
                branch = 1;

            }
                //outFile<<"J #"<<inst->immd*4<<endl;

                //cout<<"J #"<<inst->immd*4<<endl;

        
        }
        else
        {
            if(inst->op == "ADD" ||inst->op == "SUB" || inst->op == "MUL"||inst->op == "AND"||inst->op == "NOR"||inst->op == "SLT"||inst->op == "BEQ")
            {
                if(inst->op == "ADD")     // rt = rs +  immd
                {    
                    Register[inst->rt] = Register[inst->rs] + inst->immd;

                    //outFile<<inst->op<<" "<<"R"<<inst->rs<<", R"<<inst->rt<<", #"<<inst->immd*4<<endl;
                }
                else if(inst->op == "SUB")
                {                   
                    Register[inst->rt] = Register[inst->rs] - inst->immd;

                }
                else if(inst->op == "MUL")
                {
                    Register[inst->rt] = Register[inst->rs] * inst->immd;
                }
                else if(inst->op == "AND")
                {
                    Register[inst->rt] = Register[inst->rs] && inst->immd;
                }
                else if(inst->op == "NOR")
                {
                    Register[inst->rt] = !(Register[inst->rs] || inst->immd);
                }
                else if(inst->op == "SLT")
                {
                    Register[inst->rt] = (Register[inst->rs] < inst->immd)? 1: 0;
                }
                
                else if(inst->op == "BEQ")
                {
                    if(Register[inst->rs] == Register[inst->rt])
                    {
                        PC += inst->immd;
                        PC++;
                        branch = 1;
                    }
                }
                

                else
                {
                   // outFile<<inst->op<<" "<<"R"<<inst->rt<<", R"<<inst->rs<<", #"<<inst->immd<<endl;

                }    
                //cout<<inst->op<<" "<<"R"<<inst->rt<<", R"<<inst->rs<<", #"<<inst->immd<<endl;


            }    
            else if(inst->op == "BGTZ"||inst->op =="BLTZ" )
            {
              //  outFile<<inst->op<<" "<<"R"<<inst->rs<<", #"<<inst->immd*4<<endl;
                //cout<<inst->op<<" "<<"R"<<inst->rs<<", #"<<inst->immd*4<<endl;

                if(inst->op == "BLTZ")
                {
                    if(Register[inst->rs] < 0)
                    {
                        PC += inst->immd;
                        PC++;
                        branch = 1;
                    }
                }
                else if(inst->op == "BGTZ")
                {
                    if(Register[inst->rs] > 0)
                    {
                        PC += inst->immd;
                        PC++;
                        branch = 1;
                    }
                }

            }
            else if(inst->op == "LW" || inst->op == "SW")
            {
                 if(inst->op == "SW")
                {
                    mem_map[Register[inst->rs] + inst->immd] = Register[inst->rt];
                }
                else if(inst->op == "LW")
                {
                    Register[inst->rt] = mem_map[Register[inst->rs] + inst->immd];
                   // outFile_simu<<inst->rt<<"  "<<mem_map[Register[inst->rs]+inst->immd]<<"  "<<inst->immd<<endl;
                }
                //outFile<<inst->op<<" "<<"R"<<inst->rt<<", "<<inst->immd<<"(R"<<inst->rs<<")"<<endl;

                //cout<<inst->op<<" "<<"R"<<inst->rt<<", "<<inst->immd<<"(R"<<inst->rs<<")"<<endl;

            }
        }
        if(!branch)
        {
            PC++;
        }
        cycle++;
        Register_print();
        Data_print();

    }while(Inst_set[PC]->op != "BREAK");
    outFile_simu<<"--------------------"<<endl;
    outFile_simu<<"Cycle:"<<cycle + 1<<"\t"<<Inst_set[PC]->address<<"\t";
    Inst_print(Inst_set[PC], outFile_simu); 
    outFile_simu<<endl;
    Register_print();
    Data_print();
}

void Execute_print(Inst* inst)
{

}

void Register_print()
{
    outFile_simu<<"Registers"<<endl;
    outFile_simu<<"R00:";
    for(int i = 0; i < 32; i++)
    {
        if(i == 16)
            outFile_simu<<endl<<"R16:"; 
        outFile_simu<<"\t"<<Register[i];
    }
    outFile_simu<<endl<<endl;
}

void Data_print()
{
    outFile_simu<<"Data"<<endl;
    int Data_count = 0;
    int start = 0;
    
    for(start = mem_start; start <= mem_end; start += 4)
    {
        if(Data_count % 8 == 0)
            outFile_simu<<start<<":";
        outFile_simu<<"\t"<<mem_map[start];

        if(Data_count % 8 == 7)
            outFile_simu<<endl;
        Data_count++;
    }
    outFile_simu<<endl;
}

void Inst_print(Inst* inst, ofstream &outFile)
{
    if(inst->Itpye == "R")
    {
        if(inst->op == "BREAK")
            outFile<<"BREAK"<<endl;

            //cout<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rs<<", R"<<inst->rt<<endl;
        else if(inst->op == "SLL"||inst->op == "SRL")
        {
            outFile<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rt<<", #"<<inst->shamt<<endl;

        }
        else
        {
            outFile<<inst->op<<" "<<"R"<<inst->rd<<", R"<<inst->rs<<", R"<<inst->rt<<endl;
        }

     
    }
    else if(inst->Itpye == "J")
    {

        if(inst->op == "J")
            outFile<<"J #"<<inst->immd*4<<endl;

            //cout<<"J #"<<inst->immd*4<<endl;

       
    }
    else
    {
        if(inst->op == "ADD" ||inst->op == "SUB" || inst->op == "MUL"||inst->op == "AND"||inst->op == "NOR"||inst->op == "SLT"||inst->op == "BEQ")
        {
            if(inst->op == "BEQ")
            {    
                outFile<<inst->op<<" "<<"R"<<inst->rs<<", R"<<inst->rt<<", #"<<inst->immd*4<<endl;
            }
            else
            {
                outFile<<inst->op<<" "<<"R"<<inst->rt<<", R"<<inst->rs<<", #"<<inst->immd<<endl;

            }    
            //cout<<inst->op<<" "<<"R"<<inst->rt<<", R"<<inst->rs<<", #"<<inst->immd<<endl;


        }    
        else if(inst->op == "BGTZ"||inst->op =="BLTZ" )
        {
            outFile<<inst->op<<" "<<"R"<<inst->rs<<", #"<<inst->immd*4<<endl;
            //cout<<inst->op<<" "<<"R"<<inst->rs<<", #"<<inst->immd*4<<endl;

        }
        else if(inst->op == "LW" || inst->op == "SW")
        {
            outFile<<inst->op<<" "<<"R"<<inst->rt<<", "<<inst->immd<<"(R"<<inst->rs<<")"<<endl;

            //cout<<inst->op<<" "<<"R"<<inst->rt<<", "<<inst->immd<<"(R"<<inst->rs<<")"<<endl;

        }
    }
}


int string_32b_To_int(string input)
{
    int Max = 1<<31;
    int res;
    string unsiged_int = input.substr(1, 31);
    res = stoi(unsiged_int, 0, 2);

    if(input[0] == '1')
    {
        res += Max;             // 补上符号位
    }
    return res;
}