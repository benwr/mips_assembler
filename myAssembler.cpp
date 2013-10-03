#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

int main(const int argc, const char* const argv[]) {
  if (argc != 2) {
    cerr << "Usage: 'myAssembler [filename]'" << endl;
    return 1;
  } else {
    string filename(argv[1]);
    filename.append(".asm");
    ifstream in(filename.c_str());
  }

  // Label assignment
  // Translation
}
