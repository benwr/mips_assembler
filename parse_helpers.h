#ifndef parse_helpers_h
#define parse_helpers_h

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

// Index represents actual value of the register
string reg_names[32] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", 
  "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", 
  "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", 
  "$fp", "$ra"};

pair<int, int> parse_offset(string, int);
unsigned long int parse_reg(string, int);
int parse_literal(string, int);
int parse_label(string, int, map<string, unsigned long>);
bool is_number(string);
bool is_label(string);


pair<int, int> parse_offset(string arg, int line_no) {
  // parse a string of the form <numeric-literal>(<register>)  into a pair
  // {<register>,<numeric-literal>}.
  stringstream ss(arg);
  
  // The Dreaded C-Programmer's Disease! This would, unfortunately, not be 
  // worth the fix in the context of this project.
  const unsigned int max_offset_length = 50;
  char offset[max_offset_length];
  ss.get(offset, max_offset_length, '(');
  ss.get();
  string i(offset);

  // Same hackiness here.
  const unsigned int max_reg_length = 10;
  char reg[max_reg_length];
  ss.get(reg, max_reg_length, ')');
  string s(reg);

  return make_pair(parse_reg(s, line_no), parse_literal(i, line_no));
}

unsigned long int parse_reg(string arg, int line_no) {
  // Get a register number from a register specifier, either by name or directly
  // referring to the number.
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
    }
  } 
  throw (line_no);
}

int parse_literal(string arg, int line_no) {
  // Parses hexadecimal and decimal numbers

  if ((arg[0] == '0' && arg[1] == 'x') ||  // positive hex number
      (arg[0] == '-' && arg[1] == '0' && arg[2] == 'x')) { // negative hex number
    long int i;
    // Important to start counting at the right place
    int offset = (arg[0] == '-' ? 3 : 2);
    stringstream ss;
    ss << hex << arg.substr(offset);
    ss >> i;
    // Also important to actually invert the negative case
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
  // If this is a number literal, parse it as such. If not, check if it's a
  // defined label. If it is, great. Return the value it refers to. If not,
  // throw an exception.
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
  // Determines if a string is composed of decimal digits.
  if (arg[0] == '-') arg = arg.substr(1); // Disregard negative sign
  string::const_iterator it = arg.begin();
  while (it != arg.end() && isdigit(*it)) it++;
  return !(arg.empty()) && (it == arg.end());
}

bool is_label(string arg) {
  // Determine if a string is composed of alphanumeric digits.
  string::const_iterator it = arg.begin();
  while (it != arg.end() && (isalnum(*it) || *it == '_' || *it == '-')) it++;
  return !(arg.empty()) && (it == arg.end());
}

#endif
