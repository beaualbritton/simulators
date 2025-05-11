#include "instruction.h"

typedef struct
{
  int pc; 
  int next_pc;

  int* memory;
  int* registers;

}CPU;

void initialize_cpu(CPU* cpu);
int IF (CPU* cpu);
Instruction ID(int instruction);
void EX(CPU* cpu, Instruction instruction);

