#include "jack_tokenizer.hpp"

#include <sstream>
#include <unistd.h>

int main(int argc, char** argv) {
  std::string file_path(argv[1]);
  JackTokenizer tokenzier(file_path);
  while (tokenzier.hasMoreTokens()) {
    if (tokenzier.advance()) {
      
    } else {
      std::cout << "error occured: " << tokenzier.current_line() << std::endl;
      break;
    }
  }
  return 0;
}