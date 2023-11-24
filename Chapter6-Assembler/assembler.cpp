#include <iostream>
#include "assembler.hpp"

int main(int argc, char** argv) {
  std::string asm_path = std::string(argv[1]);
  Assembler assembler;
  if (!assembler.init(asm_path)) {
    LOG(Assembler) << "failed init assembler" << std::endl;
    return -1;
  }
  assembler.assembly();
  return 0;
}