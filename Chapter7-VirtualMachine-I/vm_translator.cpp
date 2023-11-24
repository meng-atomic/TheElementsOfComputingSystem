#include "vm_translator.hpp"

int main(int argc, char** argv) {
  std::string vm_dir(argv[1]);
  VMTranslator translator(vm_dir);
  if (!translator.translate()) {
    std::cerr << "Failed translate" << std::endl;
  }
  return 0;
}