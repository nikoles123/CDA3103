#include "spimcore.h"

/*Group 72 members: Gracie Bliss, Nikole Solano, and Dana Bobkova*/

/* ALU */
/* 10 Points */
void ALU(unsigned A, unsigned B, char ALUControl, unsigned * ALUresult, char * Zero)
{
  //each case is a different operation (table found in pdf explaning project)
  switch (ALUControl){
    //000         Z = A + B
    case 0:
      *ALUresult = A + B;
      break;
    //001         Z = A - B
    case 1:
      *ALUresult = A - B;
      break;
    //010         if A < B, Z = 1, otherwize Z = 0 
    case 2:
      if(A < B)
      {
        *ALUresult = 1;
      }
      else
      {
        *ALUresult = 0;
      }
      break;
    //011         if A < B, Z = 1, otherwise Z = 0 (A and B are unsigned integers)
    case 3:
      if(A < B)
      {
        *ALUresult = 1;
      }
      else
      {
        *ALUresult = 0;
      }
      break;
    //100         Z = A AND B
    case 4:
      *ALUresult = A & B;
      break;
    //101         Z = A OR B
    case 5:
      *ALUresult = A | B;
      break;
    //110         shift left B by 16 bits
    case 6:
      *ALUresult = B << 16;
      break;
    //111         Z = NOT A
    case 7:
      *ALUresult = ~A;
      break;
  }
  //if the result is 0, then the zero flag is set to 1, else it is set to 0
  if(*ALUresult == 0)
  {
    *Zero = 1;
  }
  else
  {
    *Zero = 0;
  }
}

/* instruction fetch */
/* 10 Points */
int instruction_fetch(unsigned PC, unsigned * Mem, unsigned * instruction)
{
  //check for word allignment
  if(PC % 4 == 0){
    //use PC >> 2 to get the actual location
    //Fetch the instruction addressed by PC from Mem and write it to instruction.2.
    *instruction = Mem[(PC >> 2)];
    return 0;
  }
  else{
    //Return 1 if a halt condition occurs; otherwise, return 0
    return 1; 
  }
}

/* instruction partition */
/* 10 Points */
void instruction_partition(unsigned instruction, unsigned *op, unsigned * r1, unsigned * r2, unsigned * r3, unsigned * funct, unsigned * offset, unsigned * jsec)
{
  //Partition instruction into several parts (op, r1, r2, r3, funct, offset, jsec).
  //Read line 41 to 47 of spimcore.c for more information.
  *op = (instruction >> 26); 
  if(*op == 000000){
    *r1 = (instruction >> 21);
    *r2 = (instruction >> 16);
    *r3 = (instruction >> 11);
    *funct = (instruction << 5);
    //r-type
  }
  if(*op == 000010 || *op == 000011){
    //j-type
    *jsec = (instruction << 25);
  }
  else{
    *r1 = (instruction >> 21);
    *r2 = (instruction >> 16);
    *offset = (instruction << 15);
  }
}

/* instruction decode */
/* 15 Points */
int instruction_decode(unsigned op, struct_controls * controls)
{
  //Decode the instruction using the opcode (op)
  //Assign the values of the control signals to the variables in the structure controls(See spimcore.h file).
  //The meanings of the values of the control signals:
  //For MemRead, MemWrite or RegWrite, the value 1 means that enabled, 0 means that disabled, 2 means “don’t care”.
  //For RegDst, Jump, Branch, MemtoReg or ALUSrc, the value 0 or 1 indicates theselected path of the multiplexer; 2 means “don’t care”.
  //Return 1 if a halt condition occurs; otherwise, return 0.
    controls -> RegDst = 0;
    controls -> Jump = 0;
    controls -> Branch = 0;
    controls -> MemRead = 0;
    controls -> MemtoReg = 0;
    controls -> ALUOp = 0;
    controls -> MemWrite = 0;
    controls -> ALUSrc = 0;
    controls -> RegWrite = 0;
  if(op == 0){
    //r-type format
    controls -> RegDst = 1;
    controls -> ALUOp = 7;
    controls -> RegWrite = 1;
    return 0;
  }
  if(op == 2){
    //jump instruction op code
    controls -> Jump = 1;
    return 0;
  }
  if(op == 4){
    //branch equals op code
    controls -> RegDst = 2;
    controls -> Branch = 1;
    controls -> MemtoReg = 2;
    controls -> ALUOp = 1;
    return 0;
  }
  if(op == 8){
    //add immediate op code
    controls -> ALUSrc = 1;
    controls -> RegWrite = 1;
    return 0;
  }
  if(op == 10){
    //set less than op code
    controls -> RegDst = 1;
    controls -> RegWrite = 1;
    controls -> ALUOp = 2;
    return 0;
  }
  if(op == 11){
    //set less than unsigned op code
    controls -> RegDst = 1;
    controls -> RegWrite = 1;
    controls -> ALUOp = 3;
    return 0;
  }
  if(op == 15){
    //lui op code 
    controls -> ALUSrc = 1;
    controls -> RegWrite = 1;
    controls -> ALUOp = 6;
    return 0;
  }
  if(op == 35){
    //load word op code 
    controls -> ALUSrc = 1;
    controls -> MemtoReg = 1;
    controls -> RegWrite = 1;
    controls -> MemRead = 1;
    return 0;
  }
  if(op == 43){
    //store word op code
    controls -> RegDst = 2;
    controls -> ALUSrc = 1;
    controls -> MemtoReg = 2;
    controls -> MemWrite = 1;
    return 0;
  }
  return 1;
}

/* Read Register */
/* 5 Points */
void read_register(unsigned r1, unsigned r2, unsigned * Reg, unsigned * data1, unsigned * data2)
{
  //Read the registers addressed by r1 and r2 from Reg, and write the read values todata1 and data2 respectively
  *data1 = Reg[r1];
  *data2 = Reg[r2];
}

/* Sign Extend */
/* 10 Points */
void sign_extend(unsigned offset, unsigned * extended_value)
{
  //0's if pos
  //1's if neg
  //Assign the sign-extended value of offset to extended_value

  //check if the offset is negative
  if((offset >> 15)==1){
  //fill with 1s
  * extended_value= offset | 0xffff0000;
  }
  else{
    //otherwise if its positive fill with 0s
   * extended_value= offset & 0x0000ffff;
  }
}

/* ALU operations */
/* 10 Points */
int ALU_operations(unsigned data1, unsigned data2, unsigned extended_value, unsigned funct, char ALUOp, char ALUSrc,unsigned * ALUresult, char * Zero)
{
  //Apply ALU operations on data1, and data2 or extended_value (determined by ALUSrc).

  //The operation performed is based on ALUOp and funct.
  //Apply the function ALU(...)
  //Output the result to ALUresult
  //Return 1 if a halt condition occurs; otherwise, return 0
  //check instruction passed and return 1 if it is invalid
  if(ALUOp<0 || ALUOp>7){
    return 1;
  }

  unsigned temp=ALUOp;
  
  //if it is an R type
  if(ALUOp ==7){
    switch(funct){

      //add
      case 0x20:
      temp=0;
      break;

      //subtract
      case 0x22:
      temp=1;
      break;

      //and
      case 0x24:
      temp=4;
      break;

      //or
      case 0x25:
      temp=5;
      break;

      //set less than
      case 0x2a:
      temp=2;
      break;

      //set less than unsigned
      case 0x2b:
      temp=3;
      break;

      //return 1 if its invalid
      default:
      return 1;
    }
  }

  //if its from a sign extend set data 2 to the extended value
  unsigned temp2 = (ALUSrc ==1) ? extended_value : data2;

  ALU(data1, temp2, temp, ALUresult, Zero);

  //return 0 if it is valid
  return 0;

}

/* Read / Write Memory */
/* 10 Points */
int rw_memory(unsigned ALUresult, unsigned data2, char MemWrite, char MemRead, unsigned * memdata, unsigned * Mem)
{
  //Base on the value of MemWrite or MemRead to determine memory write operation ormemory read operation.
  if(MemWrite){
    if(ALUresult % 4 == 0){
      Mem[(ALUresult >> 2)] = data2;
      return 0;
    }
    else{
      return 1;
    }
  }
  if(MemRead){
    if(ALUresult % 4 == 0){
      *memdata = Mem[(ALUresult >> 2)];
      return 0;
    }
    else{
      return 1;
    }
  }
  //Read the content of the memory location addressed by ALUresult to memdata.
  //Write the value of data2 to the memory location addressed by ALUresult.
  //Return 1 if a halt condition occurs; otherwise, return 0.
  return 1;
}

/* Write Register */
/* 10 Points */
void write_register(unsigned r2, unsigned r3, unsigned memdata, unsigned ALUresult, char RegWrite, char RegDst, char MemtoReg, unsigned * Reg)
{
  //Write the data (ALUresult or memdata) to a register (Reg) addressed by r2 or r3

  //checking if it comes from memory
  if(RegWrite==1){
    if(MemtoReg==1){
      Reg[r2]=memdata;
    }
    else
    //if it comes from a register figure out which register and write based if it is an R ot I type instruction
      if(MemtoReg==0){
        if(RegDst==1){
          Reg[r3]=ALUresult;
        }
        else{
          Reg[r2]=ALUresult;
        }
      }
  }
}

/* PC update */
/* 10 Points */
void PC_update(unsigned jsec, unsigned extended_value, char Branch, char Jump, char Zero, unsigned *PC)
{
  //Update the program counter (PC)
  //always add 4 to the program counter
  *PC = *PC + 4;

  //take care of branch
  //if we are branching zero also needs to be asserted
  //need to extend the value
  if(Branch == 1 && Zero == 1)
  {
    *PC += extended_value << 2;
  }

  //take care of jump
  //Jump: Left shift bits of jsec by 2 and use upper 4 bits of PC
  if(Jump == 1)
  {
    *PC = (jsec << 2) | (*PC & 0xf0000000);
  }
}