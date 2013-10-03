string i_type_instructions[21] = {"addi", "addiu", "andi", "beq", "bne", "lbu", 
  "lhu", "ll", "lui", "lw", "ori", "slti", "sltiu", "sb", "sc", "sh", "sw", "lwc1",
  "ldc1", "swc1", "sdc1"};

int i_type_opcodes[21] = {0x8, 0x9, 0xc, 0x4, 0x5, 0x24, 0x25, 0x30, 0xf, 0x23,
  0xd, 0xa, 0xb, 0x28, 0x38, 0x29, 0x2b, 0x31, 0x35, 0x39, 0x3d};

string i_type_argformats[21] = {"rri", "rri", "rri", "rrl", "rrl", "ra_", "ra_",
  "ra_", "r_i", "ra_", "rri", "rri", "rri", "ra_", "ra_", "ra_", "ra_", "ra_",
  "ra_", "ra_", "ra_"};
