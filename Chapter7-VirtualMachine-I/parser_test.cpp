#include "parser.hpp"

int main(int argc, char** argv) {
  std::string vm_path(argv[1]);
  Parser parser;
  parser.init(vm_path);
  while (parser.hasMoreCommands()) {
    if (parser.advance()) {
      std::cout << parser.commandType() << ", " << parser.arg1() << ", " << parser.arg2() << std::endl;
    }
  }
  return 0;
}