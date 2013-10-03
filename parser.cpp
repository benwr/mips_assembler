#include <iostream>
#include <map>
#include <set>
#include "lexer.cpp"
using namespace std;

string r_type_parse(Instruction, int);
unsigned long int parse_arg(string);

int r_type_opcodes[18] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
  0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x0, 0x0, 0x0};

int r_type_funcs[18] = {0x20, 0x21, 0x24, 0x08, 0x27, 0x25, 0x2a, 0x2b,
  0x00, 0x02, 0x22, 0x23, 0x10, 0x12, 0x0, 0x18, 0x19, 0x03};

string r_type_instructions[18] =  {"add", "addu", "and", "jr", "nor", "or", 
  "slt", "sltu", "sll", "srl", "sub", "subu", "mfhi", "mflo", "mfc0", "mult", 
  "multu", "sra"};

string reg_names[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", 
  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", 
  "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", 
  "$fp", "$ra"};

string parse(Instruction inst, map<string, unsigned long> labels) {
  for (int i = 0; i < 18; i++)
    if (inst.oper == r_type_instructions[i]){
      return r_type_parse(inst, i);
    }
  throw(inst.line_no);
}

string r_type_parse(Instruction inst, int index) {
  stringstream ss;
  ss << "opcode: " << r_type_opcodes[index] << endl;
  ss << "func: " << r_type_funcs[index] << endl;
  ss << "arg1: " << parse_arg(inst.args[0]) << endl;
  ss << "arg2: " << parse_arg(inst.args[1]) << endl;
  ss << "arg3: " << parse_arg(inst.args[2]) << endl;
  return ss.str();
}

unsigned long int parse_arg(string arg) {
  if (arg[0] == '$') {
    for (int i = 0; i < 32; i++)
      if (reg_names[i] == arg)
        return i;
  } else
    return 0;
}
