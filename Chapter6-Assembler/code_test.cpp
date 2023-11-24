#include <iostream>

#include "log.hpp"
#include "code.hpp"

int main(int argc, char** argv) {
  auto instruction = Code::dest("M");
  LOG(Test) << instruction.to_string() << std::endl;
  instruction = Code::dest("ADM");
  LOG(Test) << instruction.to_string() << std::endl;
  instruction = Code::comp("M+D");
  LOG(Test) << instruction.to_string() << std::endl;
  instruction = Code::comp("D+M");
  LOG(Test) << instruction.to_string() << std::endl;
  instruction = Code::symbol("D+M");
  LOG(Test) << instruction.to_string() << std::endl;
  instruction = Code::symbol("127");
  LOG(Test) << instruction.to_string() << std::endl;
  return 0;
}