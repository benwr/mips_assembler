#include <iostream>
#include <sstream>
#include <list>

using namespace std;

void strip_leading_whitespace(istream& in);
string extract_label(istream& in);
string extract_operator(istream& in);
string extract_argument(istream& in);

class Instruction {
  public:
  string oper;
  string args[3];
};

list<Instruction> lex(ifstream& in, map<string, unsigned long>& labels) {
  unsigned long pc = 0x00400000;
  string stringline;
  list<Instruction> result;
  while (getline(in, stringline)){
    stringstream line(stringline);

    strip_leading_whitespace(line);
    string label = extract_label(line);
    if (label.size()) {
      labels[label] = pc;
      cout << "Label: " << label << endl;
    }

    strip_leading_whitespace(line);
    Instruction inst;
    inst.oper = extract_operator(line);

    if (inst.oper.size()) {
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
      cout << "Program Counter: " << pc << endl;
      cout << "Operator: " << inst.oper << endl;
      cout << "Arg1: " << inst.args[0] << endl;
      cout << "Arg2: " << inst.args[1] << endl;
      cout << "Arg3: " << inst.args[2] << endl << endl;
    }
  }
  return result;
}

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
      (in.rdstate() & (ifstream::eofbit | ifstream::failbit))) {
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
