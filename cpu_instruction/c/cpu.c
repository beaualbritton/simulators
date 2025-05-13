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

//TODO: FINISH IMPLEMENTING 
//altering ID for more risc-v like decode, using RISCV 
//bit order instead of my own little endian, was checking highest order. now checking lowest first (for opcode)
//this is not a full risc-v implementation, so i will not be implementing funct3 and funct7 with opcodes.
//  opcode = instruction & 0x7F;
//  int parsed_opcode;
//
//  switch(opcode)
//  {
//    case 0x33: parsed_opcode = ADD; break;        
//    case 0x13: parsed_opcode = ADDI; break;       
//    case 0x63: parsed_opcode = BEQ; break;        
//    case 0x6F: parsed_opcode = JAL; break;        
//    case 0x03: parsed_opcode = LW; break;         
//    case 0x23: parsed_opcode = SW; break;         
//    case 0x67: parsed_opcode = RETURN; break;     
//    default:   parsed_opcode = NOOP; break;
//  };
//
//
//  RD = (instruction >> 7) & 0x1F;
//  RS1 = (instruction >> 15) & 0x1F;
//  RS2  = (instruction >> 20) & 0x1F;
//  immed = instruction >> 20;
//
//  //but with that in mind i have to watch out for JALR (found in line 30 of riscv_test.s) since it's not a covered opcode
//  if(opcode == 0x67)
//    opcode = 7;
//  
//  //also must keep B-type immediates in mind. For 'i' types (ADD, SW, LW) immediate is stored in the last 12 bits (from 20 to 31)
//  //so bitshift (>>) with 20 works well. NOT THE CASE FOR B-TYPES. this almost killed me 
//
//  if(opcode == 0x63)
//  {
//    int b_instruction;
//    //13 bits scattered across 32 bit address. 31-25, 11-8, 7
//    b_instruction = ((instruction >> 31) & 1) << 12;
//    b_instruction += ((instruction >> 7) & 1) << 11;
//    b_instruction += ((instruction >> 25) & 0x3F) << 5;
//    b_instruction += ((instruction >> 8) & 0xF) << 1;
//    immed = (b_instruction<<19)>>19;
//  }
  return new_instruction((OPCODE)opcode,RD,RS1,RS2,immed);
}

void write_register(CPU* cpu, int reg, int value)
{
  if(reg != 0)
    cpu->registers[reg] = value;
}

void EX(CPU* cpu, Instruction instruction)
{

 printf(">> mem@pc =[%d]@pc->%d\nEX: opcode=%d RD=%d RS1=%d RS2=%d imm=%d\n",
             cpu->memory[cpu->pc],
             cpu->pc,
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
