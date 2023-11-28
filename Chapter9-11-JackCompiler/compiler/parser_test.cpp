#include "parser.hpp"
#include "log.hpp"
#include "common.hpp"

int main(int argc, char** argv) {
  std::string file_path(argv[1]);
  JackTokenizer tokenzier(file_path);
  Parser parser;
  while (tokenzier.hasMoreTokens()) {
    if (tokenzier.advance()) {
      auto token_info = tokenzier.token_info();
      parser.add_token(token_info);
      parser.print_parse_tree();
    } else {
      std::cout << "error occured: " << tokenzier.current_line() << std::endl;
      break;
    }
  }
  return 0;
}