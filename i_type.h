/* filename: i_type.h
 * author: Ben Weinstein-Raun
 * date: 2013/10/02
 * description: Methods and data for parsing specifically I-type instructions.
 */
#ifndef i_type_h
#define i_type_h

#include <iostream>
#include <iomanip>
#include "parse_helpers.h"

string i_type_instructions[21] = {"addi", "addiu", "andi", "beq", "bne", "lbu", 
  "lhu", "ll", "lui", "lw", "ori", "slti", "sltiu", "sb", "sc", "sh", "sw", 
  "lwc1", "ldc1", "swc1", "sdc1"};

int i_type_opcodes[21] = {0x8, 0x9, 0xc, 0x4, 0x5, 0x24, 0x25, 0x30, 0xf, 0x23,
  0xd, 0xa, 0xb, 0x28, 0x38, 0x29, 0x2b, 0x31, 0x35, 0x39, 0x3d};

string i_type_argformats[21] = {"rri", "rri", "rri", "rrl", "rrl", "ra_", "ra_",
  "ra_", "r_i", "ra_", "rri", "rri", "rri", "ra_", "ra_", "ra_", "ra_", "ra_",
  "ra_", "ra_", "ra_"};

string i_type_parse(Instruction inst, 
                    int index, 
                    map<string, unsigned long> labels) {
  stringstream ss;
  int opcode = i_type_opcodes[index];

  // Default argument values
  unsigned int t = 0;
  unsigned int s = 0;
  int i = 0;
  
  // i_type_argformats are intuitive descriptors of how the arguments should be
  // arranged. Based on the predefined maps, we don't have to make guesses about
  // which kind of value is represented by each argument.
  if (i_type_argformats[index] == "rri" ) {
    t = parse_reg(inst.args[0], inst.line_no);
    s = parse_reg(inst.args[1], inst.line_no);
    i = parse_literal(inst.args[2], inst.line_no);

  } else if (i_type_argformats[index] == "ra_") {
    t = parse_reg(inst.args[0], inst.line_no);
    pair<int, int> p = parse_offset(inst.args[1], inst.line_no);
    s = p.first;
    i = p.second;

  } else if (i_type_argformats[index] == "r_i") {
    t = parse_reg(inst.args[0], inst.line_no);
    i = parse_literal(inst.args[1], inst.line_no);
    
  } else if (i_type_argformats[index] == "rrl") {
    s = parse_reg(inst.args[0], inst.line_no);
    t = parse_reg(inst.args[1], inst.line_no);
    i = (parse_label(inst.args[2], inst.line_no, labels) - inst.addr) / 4;
  }

  // Mask and shift the fields
  unsigned long int result = ((opcode & 0x3f) << 26) |
                             ((s & 0x1f) << 21) |
                             ((t & 0x1f) << 16) | 
                             (i & 0xffff);
  
  ss << setfill('0') << setw(8) << hex << result << endl;
  return ss.str();
}

#endif
