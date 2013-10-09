/* filename: instructions.h
 * author: Ben Weinstein-Raun
 * date: 10/02/2013
 * description: This file contains a simple struct for use as a tokenized view 
 *    of an instruction.
 */
#ifndef instructions_h
#define instructions_h

#include <string>
using namespace std;

struct Instruction {
  int line_no;
  string oper;
  string args[3];
  unsigned int addr;
};

#endif
