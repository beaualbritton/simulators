import instruction as instr

class cpu():
    NUM_REGISTERS = 16
    MEMORY_SIZE = 65536  # (2 ** 16) or 0xFFFF in hexadecimal

    def __init__(self):
        # setting these values to 0 initially
        self.pc = 0
        self.next_pc = 0
        # setting size of memory and register arrays (lists)
        self.memory = [0] * self.MEMORY_SIZE
        self.registers = [0] * self.NUM_REGISTERS
    
    def IF(self):
        '''
        Fetches the current instruction, and sets this object's next pc to current pc + 1
        '''
        instruction = self.memory[self.pc]
        self.next_pc = self.pc + 1
        return instruction
    
    def ID(self, instruction):
        '''
        Decodes instruction passed via parameter. Peforms multiple operations with bitwise and 
        bitshift to extract correct information. (assuming instruction is an integer)
        '''
        # bitshift operations followed by a bitwise & (and) relative to size 
        # 0XF = 15 (4 bits, 2**4 = 16-1 (minus 1 counting 0 as an index))
        opcode = (instruction >> 28) & 0XF
        RD = (instruction >> 24) & 0XF
        RS1 = (instruction >> 20) & 0XF
        RS2 = (instruction >> 16) & 0XF
        # bitwise and is size 2**16 which is 65536 (possible combinations)
        immed = instruction & 0XFFFF
        # return decoded instruction using instructions basic constructor
        return instr.instruction(opcode, RD, RS1, RS2, immed)
    
    # helper function for EX
    def write_register(self, reg_num, value):
        # R0 is always 0. just to avoid messy code in EX() 
        if reg_num != 0:
            self.registers[reg_num] = value
    
    def EX(self, instruction):
        '''
        executes instruction passed in via a parameter. assumes instruction is an
        instruction object (using default construction), not an integer (encoded instruction via build instruction)
        '''
        # do nothing (no operation)
        if instruction.opcode == instr.NOOP:
            pass
        
        # adding registers rs1 and rs2 and storing it in rd
        elif instruction.opcode == instr.ADD:
            alu_result = self.registers[instruction.RS1] + self.registers[instruction.RS2]
            self.write_register(instruction.RD, alu_result)
        # adding immediate value to rs1 and storing it in rd
        elif instruction.opcode == instr.ADDI:
            alu_result = self.registers[instruction.RS1] + instruction.immed
            self.write_register(instruction.RD, alu_result)
        # compares values in rs1 and rs2 and jumps pc to pc+immediate 
        elif instruction.opcode == instr.BEQ:
            if self.registers[instruction.RS1] == self.registers[instruction.RS2]:
                self.next_pc = self.pc + instruction.immed
        # writes to rd and jumps next pc to pc+immediate
        elif instruction.opcode == instr.JAL:
            self.write_register(instruction.RD, self.pc + 1)
            self.next_pc = self.pc + instruction.immed
        # lw according to assignment spec, with bounds for potential error
        elif instruction.opcode == instr.LW:
            eff_address = self.registers[instruction.RS1] + instruction.immed
            if 0 <= eff_address < self.MEMORY_SIZE:
                self.write_register(instruction.RD, self.memory[eff_address])
        # sw according to assignment spec, with bounds for potential error
        elif instruction.opcode == instr.SW:
            eff_address = self.registers[instruction.RS2] + instruction.immed
            if 0 <= eff_address < self.MEMORY_SIZE:
                self.memory[eff_address] = self.registers[instruction.RS1]
        # this will break the while loop in main.py
        elif instruction.opcode == instr.RETURN:
            self.next_pc = -1
