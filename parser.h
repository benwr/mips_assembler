/* filename: parser.h
 * author: Ben Weinstein-Raun
 * date: 2013-10-02
 * description: This file contains methods and data for parsing tokenized MIPS
 *    assembly code from the Instruction struct into the output format, a hex
 *    string.
 */

#ifndef parser_h
#define parser_h

#include <map>
#include "r_type.h"
#include "i_type.h"
#include "instructions.h"
using namespace std;

string parse(Instruction inst, map<string, unsigned long> labels) {
  // Takes an instruction and dispatches parsing to either the r-type parser
  // or the i-type parser.
  for (int i = 0; i < 20; i++) {
    if (inst.oper == r_type_instructions[i]){
      return r_type_parse(inst, i);
    }
  }
  
  // These for loops aren't very efficient. Perhaps a set lookup would be 
  //   better, but then again, maybe it wouldn't. The difference is linear
  //   with the length of the program we're assembling, which means it's 
  //   certainly not any kind of computational bottle neck. (since the best we
  //   can hope for in this situation is a linear solution).

  for (int i = 0; i < 21; i++) {
    if (inst.oper == i_type_instructions[i]){
        return i_type_parse(inst, i, labels);
    }
  }
  // If parse() hasn't returned yet, we don't know what the right value is.
  throw(inst.line_no);
}

#endif
