#include "cpu.h"
#include "instruction.h"
#include <stdlib.h>
#include <stdio.h>
#define  NUM_REGISTERS 16
#define MEMORY_SIZE 65536


void initialize_cpu(CPU* cpu)
{
    cpu->pc = 0;
    cpu->next_pc = 0;
    //trigger warning: calloc 
    cpu->memory = (int*) calloc(MEMORY_SIZE, sizeof(int));
    cpu->registers = (int*) calloc(NUM_REGISTERS, sizeof(int));
}

int IF(CPU* cpu)
{
  int instruction = cpu->memory[cpu->pc];
  cpu->next_pc = cpu->pc+1;
  return instruction;
}

Instruction ID(int instruction)
{
  int opcode, RD, RS1, RS2, immed;
  opcode = (instruction >> 28) & 0XF;
  RD = (instruction >> 24) & 0XF;
  RS1 = (instruction >> 20) & 0XF;
  RS2 = (instruction >> 16) & 0XF;
  immed = instruction & 0XFFFF;

  return new_instruction((OPCODE)opcode,RD,RS1,RS2,immed);
}

void write_register(CPU* cpu, int reg, int value)
{
  if(reg != 0)
    cpu->registers[reg] = value;
}

void EX(CPU* cpu, Instruction instruction)
{

  printf(">> EX: opcode=%d RD=%d RS1=%d RS2=%d imm=%d\n",
             instruction.opcode,
             instruction.RD,
             instruction.RS1,
             instruction.RS2,
             instruction.immed);
  if(instruction.opcode == NOOP)
    return;
  
  int alu_result;
  if(instruction.opcode == ADD)
  {
    alu_result = cpu->registers[instruction.RS1] + cpu->registers[instruction.RS2];
    write_register(cpu, instruction.RD, alu_result);
  }
  else if(instruction.opcode == ADDI)
  {
    alu_result = cpu->registers[instruction.RS1] + instruction.immed;  
    write_register(cpu, instruction.RD, alu_result);
  }
  else if(instruction.opcode == BEQ)
  {
    if(cpu->registers[instruction.RS1] == cpu->registers[instruction.RS2])
      cpu->next_pc = cpu->pc + instruction.immed;
  }
  else if(instruction.opcode == JAL)
  {
    write_register(cpu,instruction.RD,cpu->pc+1);
    cpu->next_pc = cpu->pc + instruction.immed; 
  }
  else if(instruction.opcode == LW)
  {
    int effective_address = cpu->registers[instruction.RS1] + instruction.immed;
    if (effective_address > 0 && effective_address < MEMORY_SIZE)
      write_register(cpu, instruction.RD, cpu->memory[effective_address]);
  }
  else if(instruction.opcode == SW)
  {
    int effective_address = cpu->registers[instruction.RS2] + instruction.immed;
    if (effective_address > 0 && effective_address < MEMORY_SIZE)
      cpu->memory[effective_address] = cpu->registers[instruction.RS1];
  }
  else if(instruction.opcode == RETURN)
    cpu->next_pc = -1;

}
