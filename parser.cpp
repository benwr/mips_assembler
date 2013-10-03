#include <iostream>
#include <iomanip>
#include <map>
#include <set>
#include "r_type.h"
#include "i_type.h"
#include "lexer.cpp"
using namespace std;

string r_type_parse(Instruction, int);
string i_type_parse(Instruction, int, map<string, unsigned long>);
pair<int, int> parse_offset(string, int);
unsigned long int parse_reg(string, int);
int parse_literal(string, int);
int parse_label(string, int, map<string, unsigned long>);
bool is_number(string);
bool is_label(string);

string reg_names[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", 
  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", 
  "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", 
  "$fp", "$ra"};

string parse(Instruction inst, map<string, unsigned long> labels) {
  for (int i = 0; i < 20; i++) {
    if (inst.oper == r_type_instructions[i]){
      return r_type_parse(inst, i);
    }
  }

  for (int i = 0; i < 21; i++) {
    if (inst.oper == i_type_instructions[i]){
        return i_type_parse(inst, i, labels);
    }
  }
  throw(inst.line_no);
}

string r_type_parse(Instruction inst, int index) {
  stringstream ss;
  unsigned int opcode = r_type_opcodes[index];
  unsigned int func = r_type_funcs[index];
  unsigned int d = 0;
  unsigned int s = 0;
  unsigned int t = 0;
  signed int shamt = 0;
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
  unsigned long int result = ((opcode & 0x3f) << 26) | 
                             ((s & 0x1f) << 21) | 
                             ((t & 0x1f) << 16) |  
                             ((d & 0x1f) << 11) |  
                             ((shamt & 0x1f) << 6) |  
                             (func & 0x3f);
  ss << setfill('0') << setw(8) << hex << result << endl;
  return ss.str();
}

string i_type_parse(Instruction inst, int index, map<string, unsigned long> labels) {
  stringstream ss;
  int opcode = i_type_opcodes[index];
  unsigned int t = 0;
  unsigned int s = 0;
  int i = 0;
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
    i = (parse_label(inst.args[2], inst.line_no, labels) - inst.addr) / 4 - 1;
  }
  unsigned long int result = ((opcode & 0x2f) << 26) |
                             ((s & 0x1f) << 21) |
                             ((t & 0x1f) << 16) | 
                             (i & 0xffff);
  
  ss << setfill('0') << setw(8) << hex << result << endl;
  return ss.str();
}

pair<int, int> parse_offset(string arg, int line_no) {
  stringstream ss(arg);
  const unsigned int max_offset_length = 50;
  char offset[max_offset_length];
  ss.get(offset, max_offset_length, '(');
  ss.get();
  string i(offset);

  const unsigned int max_reg_length = 10;
  char reg[max_reg_length];
  ss.get(reg, max_reg_length, ')');
  string s(reg);

  return make_pair(parse_reg(s, line_no), parse_literal(i, line_no));
}

unsigned long int parse_reg(string arg, int line_no) {
  if (arg[0] == '$') {
    for (int i = 0; i < 32; i++)
      if (reg_names[i] == arg)
        return i;
    int i;
    stringstream ss;
    arg = arg.substr(1);
    if (is_number(arg)) {
      ss << arg;
      ss >> i;
      return i;
    } else throw(line_no);
  } else throw (line_no);
}

int parse_literal(string arg, int line_no) {
  if ((arg[0] == '0' && arg[1] == 'x') || 
      (arg[0] == '-' && arg[1] == '0' && arg[2] == 'x')) {
    long int i;
    int offset = (arg[0] == '-' ? 3 : 2);
    stringstream ss;
    ss << hex << arg.substr(offset);
    ss >> i;
    return (arg[0] == '-' ? -i : i);
  } else if (is_number(arg)){
    long int i;
    stringstream ss;
    ss << arg;
    ss >> i;
    return i;
  }
  throw(line_no);
}

int parse_label(string arg, int line_no, map<string, unsigned long> labels) {
  try {
    return parse_literal(arg, line_no);
  } catch (int l) {
    if (is_label(arg)) {
      if (labels.count(arg))
        return labels[arg];
      else
        throw make_pair(arg, line_no);
    }
  }
}


bool is_number(string arg) {
  if (arg[0] == '-') arg = arg.substr(1);
  string::const_iterator it = arg.begin();
  while (it != arg.end() && isdigit(*it)) it++;
  return !(arg.empty()) && (it == arg.end());
}

bool is_label(string arg) {
  string::const_iterator it = arg.begin();
  while (it != arg.end() && isalnum(*it)) it++;
  return !(arg.empty()) && (it == arg.end());
}
