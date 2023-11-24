#include "vm_translator.hpp"

int main(int argc, char** argv) {
  std::string vm_dir(argv[1]);
  VMTranslator translator(vm_dir);
  translator.translate();
  return 0;
}