#ifndef r_type_h
#define r_type_h

#include <iostream>
#include <iomanip>
#include "parse_helpers.h"

using namespace std;

string r_type_instructions[20] =  {"add", "addu", "and", "jr", "nor", "or", 
  "slt", "sltu", "sll", "srl", "sub", "subu", "div", "divu", "mfhi", "mflo", 
  "mfc0", "mult", "multu", "sra"};

int r_type_opcodes[20] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0};

int r_type_funcs[20] = {0x20, 0x21, 0x24, 0x08, 0x27, 0x25, 0x2a, 0x2b,
  0x00, 0x02, 0x22, 0x23, 0x1a, 0x1b, 0x10, 0x12, 0x0, 0x18, 0x19, 0x03};

string r_type_argformats[20] = {"rrr", "rrr", "rrr", "_r_", "rrr", "rrr", "rrr",
  "rrr", "rrs", "rrs", "rrr", "rrr", "_rr", "_rr", "r__", "r__", "rr_", "_rr",
  "_rr", "rrs"};

string r_type_parse(Instruction inst, int index) {
  stringstream ss;

  unsigned int opcode = r_type_opcodes[index];
  unsigned int func = r_type_funcs[index];

  // Default field values:
  unsigned int d = 0;
  unsigned int s = 0;
  unsigned int t = 0;
  signed int shamt = 0;

  // r_type_argformats are intuitive descriptors of how the arguments should be
  // arranged. Based on the predefined maps, we don't have to make guesses about
  // which kind of value is represented by each argument.
  if (r_type_argformats[index] == string("rrr")) {
    d = parse_reg(inst.args[0], inst.line_no);
    s = parse_reg(inst.args[1], inst.line_no);
    t = parse_reg(inst.args[2], inst.line_no);

  } else if (r_type_argformats[index] == string("_r_")) {
    s = parse_reg(inst.args[0], inst.line_no);

  } else if (r_type_argformats[index] == string("rrs")) {
    d = parse_reg(inst.args[0], inst.line_no);
    t = parse_reg(inst.args[1], inst.line_no);
    shamt = parse_literal(inst.args[2], inst.line_no);

  } else if (r_type_argformats[index] == string("_rr")) {
    s = parse_reg(inst.args[0], inst.line_no);
    t = parse_reg(inst.args[1], inst.line_no);

  } else if (r_type_argformats[index] == string("r__")) {
    d = parse_reg(inst.args[0], inst.line_no);

  } else if (r_type_argformats[index] == string("rr_")) {
    d = parse_reg(inst.args[1], inst.line_no);
    s = parse_reg(inst.args[0], inst.line_no);
  }

  // Mask and shift the fields
  unsigned long int result = ((opcode & 0x3f) << 26) | 
                             ((s & 0x1f) << 21) | 
                             ((t & 0x1f) << 16) |  
                             ((d & 0x1f) << 11) |  
                             ((shamt & 0x1f) << 6) |  
                             (func & 0x3f);
  ss << setfill('0') << setw(8) << hex << result << endl;
  return ss.str();
}

#endif
