#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "lexer.cpp"
#include "parser.cpp"
using namespace std;

int main(const int argc, const char* const argv[]) {
  // Opening files
  if (argc != 2) {
    cerr << "Usage: 'myAssembler [filename]'" << endl;
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

  // Read program
  map<string, unsigned long> labels;
  list<Instruction> program = lex(in, labels);


  // Translation
  for (list<Instruction>::iterator it=program.begin();
      it != program.end(); it++) {
    try {
      out << parse(*it, labels);
    }
    catch (int e) {
      cerr << "Cannot assemble the assembly code at line " << e <<endl;
    }
  }


  in.close();
}


