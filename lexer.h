/* filename: lexer.h
 * author: Ben Weinstein-Raun
 * date: 10/02/2013
 * description: This file contains methods and data for lexical analysis (tokenizing) of
 *    MIPS assembly code into the Instruction struct for further processing.
 */
#ifndef LEXER_H
#define LEXER_H

#include <sstream>
#include <list>
#include "instructions.h"

using namespace std;

void strip_leading_whitespace(istream& in);
string extract_label(istream& in);
string extract_operator(istream& in);
string extract_argument(istream& in);


list<Instruction> lex(istream& in, map<string, unsigned long>& labels) {
  // Start at user text segment
  unsigned long pc = 0x00400000;
  string stringline;
  list<Instruction> result;
  int line_no = 0;
  while (getline(in, stringline)){
    stringstream line(stringline);
    line_no++;
    // Assigning locations to labels
    strip_leading_whitespace(line);
    string label = extract_label(line);
    if (label.size()) {
      labels[label] = pc;
    }

    // Get the operator
    strip_leading_whitespace(line);
    Instruction inst;
    inst.line_no = line_no;
    inst.oper = extract_operator(line);

    // If there was an operator, get the operands
    if (inst.oper.size()) {
      inst.addr = pc;
      for (int i = 0; i < 3; i++) {
        strip_leading_whitespace(line);
        inst.args[i] = extract_argument(line);
        if (inst.args[i] == string(",")){
          inst.args[i] = string("");
          break;
        }
      }
      result.push_back(inst);
      pc += 4;
    }
  }
  return result;
}

// The majority of the remainder of this file is somewhat self-explanatory
//
void strip_leading_whitespace(istream& in) {
  while (isspace(in.peek()))
    in.get();
}

string extract_label(istream& in) {
  char c = in.get();
  string result("");
  if (c == ':') {
    return string(":");
  } else if (isalnum(c)) {
    result = extract_label(in);
    if (result.size())
      if (result[0] == ':')
        result = string(1, c);
      else
        result.insert(0, 1, c);
    else
      in.putback(c);
  } else
    in.putback(c);

  return result;
}

string extract_operator(istream& in) {
  char c = in.get();
  string result("");
  if (isspace(c)) {
    return string(" ");
  } else if (isalpha(c)) {
    result = extract_operator(in);
    if (result.size())
      if (result[0] == ' ')
        result = string(1, c);
      else
        result.insert(0, 1, c);
    else
      in.putback(c);
  } else
    in.putback(c);

  return result;
}

string extract_argument(istream& in) {
  string result("");
  strip_leading_whitespace(in);
  char c = in.get();
  if (c == ',' || 
      c == '#' || 
      (in.rdstate() & (istream::eofbit | istream::failbit))) {
    if (c == '#')
      in.putback(c);
    return string(",");
  } 
  result = extract_argument(in);
  if (result == string(",")){
    result = string(1, c);
  } else {
    result.insert(0, 1, c);
  }

  return result;
}

#endif
