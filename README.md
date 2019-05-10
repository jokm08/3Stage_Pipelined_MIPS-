# 3Stage_Pipelined_MIPS-
Cycle-accurate simulator for a pipelined MIPS processor in C++

A cycle-accurate simulator for a pipelined MIPS processor in C++ is implemented. The simulator supports a subset of the MIPS instruction set and should model the execution of each instruction with cycle accuracy. The MIPS processor you will model has a three-stage pipeline.

The MIPS program is provided to the simulator as a text file “imem.txt” file which is used to initialize the Instruction Memory. Each line of the file corresponds to a Byte stored in the Instruction Memory in binary format, with the first line at address 0, the next line at address 1 and so on. Four contiguous lines correspond to a whole instruction. Note that the words stored in memory are in “Big-Endian” format, meaning that the most significant byte is stored first. 
We have defined a “halt” instruction as 32’b1 (0xFFFFFFFF) which is the last instruction in every “imem.txt” file. As the name suggests, when this instruction is fetched, the simulation is terminated. 

The Data Memory is initialized using the “dmem.txt” file. The format of the stored words is the same as the Instruction Memory. As with the instruction memory, the data memory addresses also begin at 0 and increment by one in each line.

The instructions that the simulator supports, and their encodings are shown in Table 1. Note that all instructions, except for “halt”, exist in the MIPS ISA.

Classes
We have defined four C++ classes that each implement one of the four major blocks in a single cycle MIPS, namely RF (to implement the register file), ALU (to implement the ALU), INSMem (to implement instruction memory), and DataMem (to implement data memory).

1.RF class: contains 32 32-bit registers defined as a private member. Remember that register $0 is always 0. ReadWrite() member function provides read and write access to the register file.

2.ALU class: implements the ALU. ALUOperation() member function performs the appropriate operation on two 32 bit operands based on ALUOP. See Table 1 for more details.

3.INSMem class: A Byte addressable memory that contains instructions. The constructor InsMem()initializes the contents of instruction memory from the file imem.txt. The member function ReadMemory() provides read access to instruction memory. An access to the instruction memory class returns 4 bytes of data; i.e., the byte pointed to by the address and the three subsequent bytes.

4.DataMem class: is similar to the instruction memory, except that it provides both read and write access.

MIPS Architecture 

A pipelined MIPS processor with a 3-stage pipeline is modeled. The first stage (Stage1) of the pipeline performs instruction fetch (IF). The second stage (Stage2) performs instruction decode/RF read (ID/RF) and execute (EX). The third stage (Stage3) performs data memory load/store (MEM) and writes back to the RF (WB). Branches are resolved in Stage2 of the processor. The processor has 1 branch delay slot: i.e., the instruction immediately after a beq is alwaysexecuted.

What the code does

A cycle-accurate simulator for the 3-stage pipelined MIPS processor is written. The simulator correctly updates the architectural state of the processor in each clock cycle. The architectural state consists of the Program Counter (PC), the Register File (RF) and the Data Memory (DataMem).
The main( ) function in the skeleton code provided has a while loop. Each iteration of this loop corresponds to one clock cycle. At the end of each clock cycle, the code must output the current PC and any writes to the RF and the Dmem to the file "results.txt" in the following format:

<32-bit PC><5-bit RFWrt. Address> <32-bit RFWrt. Data> <1-bit RF Wrt.Enabke> <32-bit Dmem Wrt. Address> <32-bit Dmem Wrt. Data> <1-bit Dmem Wrt. Enable>

Here, the RF Wrt. Valid and the Dmem Wrt. Enable bits are set to 0 if there is no write to the RF and/or Dmem in that clock cycle, respectively, and to 1 if there is a write. Note that if a Wrt Enable bit is zero, then the corresponding Address/Data bits can be set to any values.
The simulation stops as soon as the "halt" instruction is fetched in the IF stage. At this point, the OutputRF() function and OutputDataMem() functions are called and dump the contents of the RF and DMem into two files.






