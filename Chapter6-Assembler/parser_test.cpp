#include "log.hpp"
#include "parser.hpp"

int main(int argc, char** argv) {
  std::string path = std::string(argv[1]);
  Parser parser;
  parser.init(path);
  while (parser.hasMoreCommands()) {
    parser.advance();
  }
  return 0;
}