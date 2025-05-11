import cpu
import instruction

def main():
    # using two different cpus for the test cases
    CPU = cpu.cpu()
    CPU.pc = 100

    CPU_2 = cpu.cpu()
    CPU_2.pc = 100
    
    # to load instructions into memory all instructions are manually added to a list
    # using the instruction classes build_instruction() method
    # this list is later iterated through and added to memory
    test_1 = [
        instruction.instruction.build_instruction(instruction.NOOP, None, None, None, None),
        instruction.instruction.build_instruction(instruction.ADDI, 1, 0, None, 8),
        instruction.instruction.build_instruction(instruction.ADDI, 2, 0, None, 7),
        instruction.instruction.build_instruction(instruction.ADD, 3, 1, 1, None),
        instruction.instruction.build_instruction(instruction.ADD, 4, 2, 2, None),
        instruction.instruction.build_instruction(instruction.BEQ, None, 3, 4, 3),
        instruction.instruction.build_instruction(instruction.ADDI, 8, 0, None, 10),
        instruction.instruction.build_instruction(instruction.JAL, 0, None, None, 2),  
        instruction.instruction.build_instruction(instruction.ADDI, 8, 0, None, 1000),
        instruction.instruction.build_instruction(instruction.SW, None, 2, 8, 16),
        instruction.instruction.build_instruction(instruction.LW, 5, 8, None, 16),
        instruction.instruction.build_instruction(instruction.RETURN, None, None, None, None)
    ]

    test_2 = [
        instruction.instruction.build_instruction(instruction.ADDI, 1, 0, None, 5),  
        instruction.instruction.build_instruction(instruction.ADDI, 2, 0, None, 6),
        instruction.instruction.build_instruction(instruction.ADD, 3, 2, 1, None),   
        instruction.instruction.build_instruction(instruction.ADD, 4, 1, 2, None),   
        instruction.instruction.build_instruction(instruction.BEQ, None, 3, 4, 3),   
        instruction.instruction.build_instruction(instruction.ADDI, 8, 0, None, 10), 
        instruction.instruction.build_instruction(instruction.JAL, 0, None, None, 2), 
        instruction.instruction.build_instruction(instruction.ADDI, 8, 0, None, 30), 
        instruction.instruction.build_instruction(instruction.SW, None, 3, 8, 10),   
        instruction.instruction.build_instruction(instruction.LW, 5, 8, None, 10),   
        instruction.instruction.build_instruction(instruction.RETURN, None, None, None, None) 
    ]
    
    # adding instructions to memory
    for i, instr in enumerate(test_1):
        CPU.memory[100+i] = instr

    # main loop, using CPU's built in method to encapsulate fetching, decoding and executing.
    while CPU.next_pc != -1:
        # fetch 
        fetched_instruction = CPU.IF()
        # decode
        decoded_instruction = CPU.ID(fetched_instruction)
        # execute
        CPU.EX(decoded_instruction)
        CPU.pc = CPU.next_pc
    
    # fancy printing to view registers
    for i, register in enumerate(CPU.registers):
        print(f"r{i}={register}")
    # and also memory
    print(CPU.memory[26])

    # below is the same code as above but for test case 2
    for i, instr in enumerate(test_2):
        CPU_2.memory[100+i] = instr

    while CPU_2.next_pc != -1:
        # fetch 
        fetched_instruction = CPU_2.IF()
        # decode
        decoded_instruction = CPU_2.ID(fetched_instruction)
        # execute
        CPU_2.EX(decoded_instruction)
        CPU_2.pc = CPU_2.next_pc
    
    for i, register in enumerate(CPU_2.registers):
        print(f"r{i}={register}")

    print(CPU_2.memory[40])

# now running the main loop
if __name__ == "__main__":
    main()
