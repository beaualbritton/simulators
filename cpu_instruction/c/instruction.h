#ifndef INSTRUCTION_H
#define INSTRUCTION_H
typedef enum 
{
  NOOP = 0,
  ADD = 1,
  ADDI = 2,
  BEQ = 3,
  JAL = 4,
  LW = 5,
  SW = 6,
  RETURN = 7,
}OPCODE;

typedef struct 
{
  int RD, RS1, RS2, immed;
  OPCODE opcode;
}Instruction;

Instruction new_instruction(OPCODE opcode, int RD, int RS1, int RS2, int immed);
int build_instruction(int opcode, int RD, int RS1, int RS2, int immed);

#endif
