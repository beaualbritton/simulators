#include <corecrt.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "cpu.h"
#include "instruction.h"
#define  NUM_REGISTERS 16
#define MEMORY_SIZE 65536
void load_test(CPU* cpu, const char* filepath);

int main()
{
  CPU cpu1,cpu2, cpu3;

  initialize_cpu(&cpu1);
  initialize_cpu(&cpu2);
//  initialize_cpu(&cpu3);
  
  cpu1.pc = 100;
  cpu2.pc = 100;
//  cpu3.pc = 100;
  
  int test1[] =
  {
      build_instruction(NOOP, -1, -1, -1, -1),
      build_instruction(ADDI, 1, 0, -1, 8),
      build_instruction(ADDI, 2, 0, -1, 7),
      build_instruction(ADD, 3, 1, 1, -1),
      build_instruction(ADD, 4, 2, 2, -1),
      build_instruction(BEQ, -1, 3, 4, 3),
      build_instruction(ADDI, 8, 0, -1, 10),
      build_instruction(JAL, 0, -1, -1, 2),
      build_instruction(ADDI, 8, 0, -1, 1000),
      build_instruction(SW, -1, 2, 8, 16),
      build_instruction(LW, 5, 8, -1, 16),
      build_instruction(RETURN, -1, -1, -1, -1)
  };
    
  int test2[] = {
    build_instruction(ADDI, 1, 0, -1, 5),
    build_instruction(ADDI, 2, 0, -1, 6),
    build_instruction(ADD, 3, 2, 1, -1),
    build_instruction(ADD, 4, 1, 2, -1),
    build_instruction(BEQ, -1, 3, 4, 3),
    build_instruction(ADDI, 8, 0, -1, 10),
    build_instruction(JAL, 0, -1, -1, 2),
    build_instruction(ADDI, 8, 0, -1, 30),
    build_instruction(SW, -1, 3, 8, 10),
    build_instruction(LW, 5, 8, -1, 10),
    build_instruction(RETURN, -1, -1, -1, -1)
  };
  for(int i = 0; i < sizeof(test1)/sizeof(int); ++i)
  {
    cpu1.memory[100+i] = test1[i];
  };
  for(int i = 0; i < sizeof(test2)/sizeof(int); ++i)
  {
    cpu2.memory[100+i] = test2[i];
  };

  //hex formatting for mem address  
  for(int i = 0; i < sizeof(test1)/sizeof(int); ++i) {
      printf("mem1[%d] = 0x%08X\n", 100 + i, cpu1.memory[100 + i]);
  }
  for(int i = 0; i < sizeof(test2)/sizeof(int); ++i) {
      printf("mem2[%d] = 0x%08X\n", 100 + i, cpu2.memory[100 + i]);
  }


  bool running = true;
  while(running)
  {
    int fetched_instruction;
    Instruction decoded_instruction;
    fetched_instruction = IF(&cpu1);
    decoded_instruction = ID(fetched_instruction);
    EX(&cpu1, decoded_instruction);
    cpu1.pc = cpu1.next_pc;
    if(cpu1.next_pc == -1)
        running = false;
  }
  for (int i = 0; i < NUM_REGISTERS; i++) {
      printf("r%d=%d\n", i, cpu1.registers[i]);
  }
  printf("%d\n", cpu1.memory[26]);

  bool cpu2running = true;
  
  while(cpu2running)
  {
    int fetched_instruction;
    Instruction decoded_instruction;
    fetched_instruction = IF(&cpu2);
    decoded_instruction = ID(fetched_instruction);
    EX(&cpu2, decoded_instruction);
    cpu2.pc = cpu2.next_pc;
    if(cpu2.next_pc == -1)
        cpu2running = false;
  }
  for (int i = 0; i < NUM_REGISTERS; i++) {
      printf("r%d=%d\n", i, cpu2.registers[i]);
  }
  printf("%d\n", cpu2.memory[40]);

  //load test 3
/*
 *TODO: FIX test 3... tried my best but lowkey wack
 */
//  load_test(&cpu3,"../tests/program.mem");
//
//  cpu3.pc = 100;
//  bool cpu3running = true;
//  while(cpu3running)
//  {
//    int fetched_instruction;
//    Instruction decoded_instruction;
//    fetched_instruction = IF(&cpu3);
//    decoded_instruction = ID(fetched_instruction);
//    printf("%d",cpu3.memory[cpu3.pc]);
//    EX(&cpu3, decoded_instruction);
//    cpu3.pc = cpu3.next_pc;
//    if(cpu3.next_pc == -1)
//      cpu3running = false;
//   
//  }

  free(cpu1.memory);
  free(cpu1.registers);
  free(cpu2.memory);
  free(cpu2.registers);
//  free(cpu3.memory);
//  free(cpu3.registers);
  return 0;
}
/*
 * TODO: fix memory bugs for load_test (particularly /tests/program.mem) cause. End up being pretty drastic on system (stuck in an infinite branching loop, has to do with byte ordering on B-type opcodes. Thanks riscv :-)
 */
void load_test(CPU* cpu, const char* filepath)
{

  //chopped im not great at c asof 5/25
  FILE* test_file;
  errno_t e = fopen_s(&test_file, filepath, "r");
  char line[256];
  int counter = 0;
  while(fgets(line, sizeof(line), test_file))
  {
    unsigned int byte1,byte2,byte3,byte4;
    byte1 = 0;byte2 = 0;byte3 = 0;byte4 = 0;
    int scanned_line = sscanf_s(line, "%x %x %x %x", &byte1,&byte2,&byte3,&byte4);

    unsigned int instruction = (byte1) | (byte2 << 8) | (byte3 << 16) | (byte4 << 24);
//    cpu->memory[cpu->pc+counter] = instruction;
//    ++counter;
    printf("%u\n", instruction);
  }
  fclose(test_file);
}
