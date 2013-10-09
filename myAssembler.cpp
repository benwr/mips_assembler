/* filename: myAssembler.cpp
 * author: Ben Weinstein-Raun
 * date: 10/02/2013
 * description: This file contains the main method for my MIPS assembler for
 *    ECE2500: Computer Organization and Architecture. To use this project,
 *    compile this file and run the resulting binary as specified in the 
 *    project outline.
 */

#include <fstream>
#include <string>
#include <map>
#include "lexer.h"
#include "parser.h"

using namespace std;

int main(const int argc, const char* const argv[]) {
  // Opening files
  if (argc != 2) {
    cerr << "Usage: 'myAssembler [program name]'" << endl;
    return 1;
  }
  string filename(argv[1]);
  string inname = filename;
  string outname = filename;
  inname.append(".asm");
  outname.append(".out");

  ifstream in;
  in.open(inname.c_str());

  ofstream out;
  out.open(outname.c_str(), fstream::binary);

  // Read program, split it into understandable parts, and find labels.
  map<string, unsigned long> labels;
  list<Instruction> program = lex(in, labels);


  // Translate the tokenized program into hexadecimal values
  for (list<Instruction>::iterator it=program.begin();
      it != program.end(); it++) {
    try {
      out << parse(*it, labels);
    } catch (int e) {
      cerr << "Cannot assemble the assembly code at line " << e <<endl;
      break;
    } catch (pair<string, int> p) {
      cerr << "Undefined label " << p.first << " at line " << p.second << endl;
      break;
    }
  }

  in.close();
  out.close();
}
