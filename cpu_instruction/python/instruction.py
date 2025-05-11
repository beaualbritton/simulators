# constant opcodes to be accessed later
NOOP = 0
ADD = 1
ADDI = 2
BEQ = 3
JAL = 4
LW = 5
SW = 6
RETURN = 7

class instruction():
    # basic constructor, takes all values without operating on them
    def __init__(self, opcode, RD, RS1, RS2, immed):
        self.opcode = opcode
        self.RD = RD
        self.RS1 = RS1
        self.RS2 = RS2
        self.immed = immed
    
    def build_instruction(opcode, RD, RS1, RS2, immed):
        '''
        this function builds an instruction to be later handled by the cpu using 
        bitshift operation on passed in values (assumed to be integers)
        '''
        instruction = opcode << 28
        # checking for null values before proceeding (opcode always has a value)
        if RD is not None:
            instruction += (RD << 24)
        if RS1 is not None:
            instruction += (RS1 << 20)
        if RS2 is not None:
            instruction += (RS2 << 16)
        if immed is not None:
            # using bitwise and with 0xFFFF (2 ** 16) to get the last 16 bits of
            # information from the current instruction
            instruction += immed & 0XFFFF
        return instruction
