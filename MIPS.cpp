#include<iostream>
#include<string>
#include<vector>
#include<bitset>
#include<fstream>
using namespace std;
#define ADDU 1
#define SUBU 3
#define AND 4
#define OR  5
#define NOR 7
#define MemSize 65536 // memory size, in reality, the memory size should be 2^32, but for this lab, for the space resaon, we keep it as this large number, but the memory is still 32-bit addressable.
#define DUMMY_VALUE (2147483649)

class RF
{
    public:
        bitset<32> ReadData1, ReadData2; 
     	RF()
    	{ 
          Registers.resize(32);  
          Registers[0] = bitset<32> (0);  
        }
	
        void ReadWrite(bitset<5> RdReg1, bitset<5> RdReg2, bitset<5> WrtReg, bitset<32> WrtData, bitset<1> WrtEnable)
        {   
            int Rd_Reg_1,Rd_Reg_2,Wrt_Reg;

            if(WrtEnable == 1)
            {
               //Get corresponding register in integer form
               Wrt_Reg = WrtReg.to_ulong();
 
               //Write data to the register
	       Registers[Wrt_Reg] = bitset<32> (WrtData);

	       cout<<"\nData Written to register"<<Wrt_Reg<<" = "<<WrtData.to_ulong();
               
            }            
            else
            {
               //Get corresponding register number in integer form
               Rd_Reg_1 = RdReg1.to_ulong(); 
               Rd_Reg_2 = RdReg2.to_ulong();

               // Write the value in the register to publc element ReadData1
               ReadData1 = Registers[Rd_Reg_1];
               cout<<"\nRegister "<<Rd_Reg_1<<" :"<<ReadData1.to_ulong(); 
               ReadData2 = Registers[Rd_Reg_2];
               cout<<"\nRegister "<<Rd_Reg_2<<" :"<<ReadData2.to_ulong();
               
            }
              
	    return;       
         }
		 
	void OutputRF()
             {
               ofstream rfout;
                  rfout.open("RFresult.txt",std::ios_base::app);
                  if (rfout.is_open())
                  {
                    rfout<<"A state of RF:"<<endl;
                  for (int j = 0; j<32; j++)
                      {        
                        rfout << Registers[j]<<endl;
                      }
                     
                  }
                  else cout<<"Unable to open file";
                  rfout.close();
               
               }     
	private:
            vector<bitset<32> >Registers;
	
};

class ALU
{
      public:
             bitset<32> ALUresult;
             bitset<32> ALUOperation (bitset<3> ALUOP, bitset<32> operand1, bitset<32> operand2)
             {   


                 long temp = 0;
	         cout<<"\nOperand1"<<operand1.to_ulong()<<"\nOperand2"<<operand2.to_ulong();

                 switch(ALUOP.to_ulong())
                 {
                    case 1:
			//Add unsigned
			cout<<"\nAdd Unsigned";
			temp = operand1.to_ulong() + operand2.to_ulong();
                        ALUresult = bitset<32> (temp);
			break;
		    case 3:
			//Sub unsigned
			cout<<"\nSub unsigned";
                        temp = operand1.to_ulong() - operand2.to_ulong();
			ALUresult = bitset<32> (temp);
			break;
                    case 4:
			//And
			cout<<"\nAND";
			temp = operand1.to_ulong() & operand2.to_ulong();
                        ALUresult = bitset<32> (temp);
			break;
		    case 5:
			//Or
			cout<<"\nOr";
			temp = operand1.to_ulong() | operand2.to_ulong();
                        ALUresult = bitset<32> (temp);
			break;
		    case 7:
			//NOR
			cout<<"\nNOR";
			temp = operand1.to_ulong() | operand2.to_ulong();
                        temp = ~(temp);
                        ALUresult = bitset<32> (temp);
			break;
                    
                 }
                 cout<<"\nResult"<<ALUresult;
                 return ALUresult;
               }            
};

class INSMem
{
      public:
          bitset<32> Instruction;
          INSMem()
          {       IMem.resize(MemSize); 
                  ifstream imem;
                  string line;
                  int i=0;
                  imem.open("imem.txt");
                  if (imem.is_open())
                  {
                  while (getline(imem,line))
                     {      
                        IMem[i] = bitset<8>(line);
                        i++;
                     }
                     
                  }
                  else cout<<"Unable to open file";
                  imem.close();
                     
                  }
                  
          bitset<32> ReadMemory (bitset<32> ReadAddress) 
              {    
               int i = 0, index;
               long temp_dec,Int_Read_Address ;
               bitset<32> Temp_Instruction = bitset<32> (0); //Initialize to 0 all 32 bits               

               Int_Read_Address = ReadAddress.to_ulong();
               
               for(i=0;i<4;i++)
               {  
                 
                  Temp_Instruction <<=8; //Shift by 8 bits to the left(4 shifts required to obtain Big Endian format)
                  
                  index = Int_Read_Address + i;
                  
                  temp_dec = IMem[index].to_ulong();
                      
                  Temp_Instruction |= bitset<32> (temp_dec);//current byte OR with left shifted previous bytes
               }
           
               Instruction = Temp_Instruction;
                             
               return Instruction;
     
              }     
      
      private:
           vector<bitset<8> > IMem;
      
};
      
class DataMem    
{
      public:
          bitset<32> readdata;  
          DataMem()
          {
             cout<<"Data memory";
             DMem.resize(MemSize); 
             ifstream dmem;
             string line;
             int i=0;
             dmem.open("dmem.txt");
             if (dmem.is_open())
             {
              while (getline(dmem,line))
              {
               DMem[i] = bitset<8>(line);
               i++;
              }
             }
             else cout<<"Unable to open file";
             dmem.close();
          
          }  
          bitset<32> MemoryAccess (bitset<32> Address, bitset<32> WriteData, bitset<1> readmem, bitset<1> writemem) 
          {    
               long Data_Address,temp_dec,index;
               int i,j =3;
               bitset<32> temp_data = bitset<32> (0);
               bitset<32> temp1 = bitset<32> (0);
               bitset<32> temp2 = bitset<32> (0);
	       bitset<32> mask = bitset<32> (4278190080);
               
               Data_Address = Address.to_ulong();

               if(readmem == 1)
               {
                
                 for(i=0; i<4; i++)
                 {
                   temp_data <<=8;
                   index = Data_Address++;
                   temp_dec = DMem[index].to_ulong();
                   temp_data |= bitset<32> (temp_dec);
                 }  
                  
                 readdata = temp_data;
               }

               if(writemem == 1)
               {
                 for(i=0; i<4; i++)
                 {
                   //Extract the most significant byte first since big endian format is used
                   temp1 = WriteData & mask;
                   mask >>= 8;//Shift mask by 8 bits so that next significany byte can be extracted
                   temp2 = temp1>>((j-i)*8);//Shift extrcted byte end byte to enable storage in byte format
                   temp_dec = temp2.to_ulong();
                   index = Data_Address++;
                   DMem[index] = bitset<8>(temp_dec);
		   cout<<"\nByte "<<index<<" :"<<DMem[index];
                 }
                 readdata = bitset<32> (0);
               }
               return readdata;     
          }   
                     
          void OutputDataMem()
          {
               ofstream dmemout;
                  dmemout.open("dmemresult.txt");
                  if (dmemout.is_open())
                  {
                  for (int j = 0; j< 1000; j++)
                      { 
			cout<<"\n"<<DMem[j];    
                        dmemout << DMem[j]<<endl;
                       }
                     
                  }
                  else cout<<"Unable to open file";
                  dmemout.close();
               
               }             
      
      private:
           vector<bitset<8> > DMem;
      
};  

class Decoder
{
	public:
	bitset<6> OpCode;
        bitset<3> ALUOp;

	void InstDecoder(bitset<32> Instruction)
        {
             bitset<32> mask_OpCode = bitset<32> (4227858432);
             bitset<32> mask_JumpAddress = bitset<32> (67108863); 
	     bitset<32> mask_ALUOp = bitset<32> (7);
             bitset<32> temp;

	     //Determining Opcode
             temp = Instruction & mask_OpCode;
             temp = temp>>26;
             OpCode = bitset<6> (temp.to_ulong()); 
             if(OpCode == (bitset<6> (0)))
	     {
               temp = Instruction & mask_ALUOp;
               ALUOp = bitset<3> (temp.to_ulong());  
             }

              
        }
};

void dumpResults(bitset<32> pc, bitset<5> WrRFAdd, bitset<32> WrRFData, bitset<1> RFWrEn, bitset<32> WrMemAdd, bitset<32> WrMemData, bitset<1> WrMemEn)
{
                  ofstream fileout;
                  
                  fileout.open("Results.txt",std::ios_base::app);
                  if (fileout.is_open())
                  {
                  
                  fileout <<pc<<' '<<WrRFAdd<<' '<<WrRFData<<' '<<RFWrEn<<' '<<WrMemAdd<<' '<<WrMemData<<' '<<WrMemEn << endl;
                     
                  }
                  else cout<<"Unable to open file";
                  fileout.close();

}

   
int main()
{   
    bitset<32> pc=0,CurrentPC = 0;
    RF myRF;
    ALU myALU;
    INSMem myInsMem;
    DataMem myDataMem;
    bitset<1>  RFWrEn         = bitset<1>  (0);
    bitset<1>  WrMemEn        = bitset<1>  (0);
    bitset<32> NextInst       = bitset<32> (DUMMY_VALUE);
    bitset<32> PrevInst       = bitset<32> (DUMMY_VALUE);
    bitset<32> WrRFData       = bitset<32> (0x00000000);
    bitset<32> WrMemAdd       = bitset<32> (0x00000000);
    bitset<32> WrMemData      = bitset<32> (0x00000000);
    Decoder myDecoder;
    bitset<6> myOpCode        = bitset<6>  (DUMMY_VALUE);
    bitset<5> WrRFAdd         = bitset<5> (0);
    bitset<5> RegisterS, RegisterT,RegisterD;
    bitset<3> myALUOp; 
    bitset<32> mask_RegisterS = bitset<32> (0x03E00000);
    bitset<32> mask_RegisterT = bitset<32> (0x001F0000);
    bitset<32> mask_RegisterD = bitset<32> (0x0000F800);
    bitset<32> mask_Immediate = bitset<32> (0x0000FFFF);
    bitset<32> mask_Address   = bitset<32> (0x03FFFFFF);
    bitset<32> ALUResult      = bitset<32> (0x00000000);
    bitset<32> SW_Address     = bitset<32> (0x00000000);
    bitset<32> temp_data      = bitset<32> (0x00000000);
    bitset<32> temp_address   = bitset<32> (0x00000000);
    bitset<32> branch_address = bitset<32> (0x00000000);
    long temp,temp_Immediate;
    int exit_code= 0,process_begin = 0,JumpDetected = 0,BranchDetected = 0;
  
    while (1) //each loop body corresponds to one clock cycle.
	{

	    WrRFAdd   = bitset<5>  (0);
            WrRFData  = bitset<32> (0x00000000);
            RFWrEn    = bitset<1>  (0);

            WrMemAdd  = bitset<32> (0x00000000);
            WrMemData = bitset<32> (0x00000000);
            WrMemEn   = bitset<1>  (0);


            /*----------------------------------------------------------------------------------------------------------------*/
	    /*Stage 3 : Data Memory Load/Store*/
	    switch(myOpCode.to_ulong())
            {
            case 35:
                  // Load Word Instruction
                  cout<<"\nLoad Word ";
                  temp = PrevInst.to_ulong() & mask_RegisterS.to_ulong();
                  temp = temp>>21;
                  RegisterS = bitset<5>(temp);
                  temp = PrevInst.to_ulong() & mask_RegisterT.to_ulong();
                  temp = temp>>16;
                  RegisterT = bitset<5>(temp);
                  cout<<"Data to be written to "<<RegisterT.to_ulong();
                  temp_Immediate = PrevInst.to_ulong() & mask_Immediate.to_ulong();
		  if(((temp_Immediate & 0x8000)>>(15)) == 1)
		  {
		    temp_Immediate = 0xFFFF0000 | temp_Immediate;
		  }
                  myRF.ReadWrite(RegisterS,(bitset<5> (0)),(bitset<5> (0)),(bitset<32> (0)), (bitset<1> (0)));
                  temp = myRF.ReadData1.to_ulong() + temp_Immediate;
                  cout<<"\nData memory Address accessed :"<<temp;
                  temp_data = myDataMem.MemoryAccess(bitset<32> (temp),bitset<32> (0), bitset<1> (1), bitset<1> (0));
                  cout<<"\nData : "<<temp_data.to_ulong();
                  myRF.ReadWrite(bitset<5> (0),bitset<5> (0),RegisterT,temp_data,bitset<1> (1));
                  break;                 
                  
                  
	    case 43:
                  //Store Word Instruction	
                  cout<<"Store Word";
                  temp = PrevInst.to_ulong() & mask_RegisterS.to_ulong();
                  temp = temp>>21;
                  RegisterS = bitset<5>(temp);
                  temp = PrevInst.to_ulong() & mask_RegisterT.to_ulong();
                  temp = temp>>16;
        	  RegisterT = bitset<5>(temp);
                  temp_Immediate = PrevInst.to_ulong() & mask_Immediate.to_ulong();
		  if(((temp_Immediate & 0x8000)>>(15)) == 1)
            	  { 
		    temp_Immediate = 0xFFFF0000 | temp_Immediate;
              	  }                
		  myRF.ReadWrite(RegisterS,RegisterT,(bitset<5> (0)),(bitset<32> (0)),(bitset<1> (0)));
                  temp = myRF.ReadData1.to_ulong() + temp_Immediate;
                  myDataMem.MemoryAccess(bitset<32> (temp),myRF.ReadData2,bitset<1> (0), bitset<1> (1));
		  WrMemAdd  = bitset<32> (temp);
                  WrMemData = bitset<32> (myRF.ReadData2);
                  WrMemEn   = bitset<1>  (1);   
 		  break;
	    }

            /*------------------------------------------------------------------------------------------------------------*/
            /*Stage 2 : Instruction Decode/ RF Read/ Execute*/
            if(process_begin == 1)
            { 
                  myDecoder.InstDecoder(NextInst);
                  myOpCode = myDecoder.OpCode;
                  myALUOp = myDecoder.ALUOp;
                  cout<<"\nOpcode : "<<myOpCode.to_ulong();
		  PrevInst = NextInst;
            }

            switch(myOpCode.to_ulong())
	    {
                  case 0:
		  // Register type instuction
		  cout<<"\nStart Execution";
                  temp = NextInst.to_ulong() & mask_RegisterS.to_ulong();
                  temp = temp>>21;
                  RegisterS = bitset<5>(temp);
                  cout<<"\nRegisterS = "<<RegisterS.to_ulong();
		  temp = NextInst.to_ulong() & mask_RegisterT.to_ulong();
                  temp = temp>>16;
                  RegisterT = bitset<5>(temp);
                  cout<<"\nRegisterT = "<<RegisterT.to_ulong();
                  temp = NextInst.to_ulong() & mask_RegisterD.to_ulong();
                  temp = temp>>11;
                  RegisterD = bitset<5>(temp); 
                  cout<<"\nRegisterD = "<<RegisterD.to_ulong();
		  myRF.ReadWrite(RegisterS, RegisterT, (bitset<5> (0)), (bitset<32> (0)), (bitset<1> (0)));
                  ALUResult = myALU.ALUOperation(myALUOp,myRF.ReadData1,myRF.ReadData2);
                  cout<<"Result :"<<ALUResult.to_ulong();
                  myRF.ReadWrite((bitset<5> (0)),(bitset<5> (0)),RegisterD,ALUResult,(bitset<1> (1)));
                  WrRFAdd  = RegisterD;
                  WrRFData = ALUResult;
                  RFWrEn   = bitset<1> (1);
                  break;

		  case 2:
		  //Jump Type instruction
		  temp_address = NextInst.to_ulong() & mask_Address.to_ulong();
                  JumpDetected = 1;
		  break;

		  case 4:
		  //Branch type instruction
		  BranchDetected = 1;
		  branch_address = NextInst.to_ulong() & mask_Immediate.to_ulong(); 
		  break;

		  case 9:
		  // Add Immediate Unsigned
		  temp = NextInst.to_ulong() & mask_RegisterS.to_ulong();
		  temp = temp>>21;
                  RegisterS = bitset<5>(temp);
      		  temp = NextInst.to_ulong() & mask_RegisterT.to_ulong();
		  temp = temp>>16;
		  RegisterT = bitset<5>(temp);
		  temp_Immediate = NextInst.to_ulong() & mask_Immediate.to_ulong();
		  if(((temp_Immediate & 0x8000)>>15) == 1)
		  {
          	    temp_Immediate = 0xFFFF0000 | temp_Immediate;
		  }
		  myRF.ReadWrite(RegisterS, (bitset<5> (0)), (bitset<5> (0)),(bitset<32> (0)), (bitset<1> (0)));
		  ALUResult = myALU.ALUOperation((bitset<3> (1)),myRF.ReadData1,(bitset<32> (temp_Immediate)));
                  myRF.ReadWrite((bitset<5> (0)),(bitset<5> (0)), RegisterT, ALUResult, (bitset<1> (1)));
		  WrRFAdd = RegisterT;
 		  WrRFData = ALUResult;
  		  RFWrEn = bitset<1> (1);
		  break;

                  case 63:
                  //Terminate process
                  exit_code = 1;
             }

             if(exit_code == 1)
             break;

            /*------------------------------------------------------------------------------------------------------------*/ 
	    /*Stage 1 : Instruction Fetch*/

            CurrentPC = pc;
	    if(!JumpDetected)
	    {
              process_begin = 1;
	      NextInst = myInsMem.ReadMemory(pc);
	      if(BranchDetected)
	      { 
		pc = bitset<32> (pc.to_ulong() + branch_address.to_ulong()); 
              }
	      else
              {
                pc = bitset<32> (pc.to_ulong() + (4));
              }
              cout<<"\nStart Fetch";
            }
            else
            {
             NextInst = myInsMem.ReadMemory(temp_address);
	     pc = bitset<32> (temp_address.to_ulong() + (4));
	     JumpDetected = 0;
            }
            

            /*------------------------------------------------------------------------------------------------------------*/
  
	
    // The first dumped pc value should be 0.	
    dumpResults(CurrentPC,WrRFAdd,WrRFData,RFWrEn,WrMemAdd,WrMemData,WrMemEn);      
    }
	  myRF.OutputRF(); // dump RF; 
          myDataMem.OutputDataMem(); // dump data mem
      
          return 0;
        
}
