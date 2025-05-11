#include "instruction.h"
Instruction new_instruction(OPCODE opcode, int RD, int RS1, int RS2, int immed)
{
    return (Instruction){
        .opcode = opcode,
        .RD     = RD,
        .RS1    = RS1,
        .RS2    = RS2,
        .immed  = immed
    };
}

int build_instruction(int opcode, int RD, int RS1, int RS2, int immed)
{
  int instruction = opcode << 28;

  if(RD!=-1)
    instruction+=(RD << 24);
  if(RS1!=-1)
    instruction+=(RS1 << 20);
  if(RS2!=-1)
    instruction+=(RS2 << 16);
  if(immed!=-1)
    instruction+=(immed & 0xFFFF);
  return instruction;
}
