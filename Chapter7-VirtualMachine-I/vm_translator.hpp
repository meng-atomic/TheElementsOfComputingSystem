#pragma once

#include <filesystem>

#include "common.hpp"
#include "parser.hpp"
#include "code_writer.hpp"

class VMTranslator {
public:
  VMTranslator(const std::string& vm_dir);
  bool translate();

private:
  void get_all_vm_files(const std::string& dir);
  std::string _dir_name;
  std::vector<std::string> _vm_files;
  CodeWriter _writer;
};

VMTranslator::VMTranslator(const std::string& vm_dir) {
  auto pos = vm_dir.find_last_not_of('/');
  std::string real_vm_dir = vm_dir;
  if (pos != std::string::npos) {
    real_vm_dir = vm_dir.substr(0, pos + 1);
  }
  pos = real_vm_dir.find_last_of('/');
  if (pos != std::string::npos) {
    _dir_name = real_vm_dir.substr(pos + 1);
  } else {
    _dir_name = real_vm_dir;
  }
  auto asm_name = real_vm_dir + "/" + _dir_name;
  _writer.init(asm_name);

  get_all_vm_files(real_vm_dir);
  for (auto& path : _vm_files) {
    LOG(Translator) << path << std::endl;
  }
}

bool VMTranslator::translate() {
  for (auto& vm_file : _vm_files) {
    Parser parser;
    if (!parser.init(vm_file)) {
      return false;
    }
    if (!_writer.setFileName(vm_file)) {
      return false;
    }
    while (parser.hasMoreCommands()) {
      if (parser.advance()) {
        CMDType cmd_type = parser.commandType();
        if (cmd_type == CMDType::C_PUSH || cmd_type == CMDType::C_POP) {
          if (!_writer.writePushPop(cmd_type, parser.arg1(), parser.arg2())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_ARITHMETIC) {
          if (!_writer.writeArithmetic(parser.arg1())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_LABEL) {
          if (!_writer.writeLabel(parser.arg1())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_GOTO) {
          if (!_writer.writeGoto(parser.arg1())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_IF) {
          if (!_writer.writeIf(parser.arg1())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_CALL) {
          if (!_writer.writeCall(parser.arg1(), parser.arg2())) {
            return false;
          }
        } else if (cmd_type == CMDType::C_RETURN) {
          if (!_writer.writeReturn()) {
            return false;
          }
        } else if (cmd_type == CMDType::C_FUNCTION) {
          if (!_writer.writeFunction(parser.arg1(), parser.arg2())) {
            return false;
          }
        } else {
          LOG(VMTranslator) << "unknown command: " << std::endl;
          return false;
        }
      } else {
        break;
      }
    }   
  }
  return true;
}

void VMTranslator::get_all_vm_files(const std::string& path) {
  if (std::filesystem::is_directory(path)) {
    for (const auto& entry : std::filesystem::directory_iterator(path)) {
      get_all_vm_files(entry.path());
    }
  } else if (std::filesystem::is_regular_file(path)) {
    if (path.size() > 3) {
      if (path.substr(path.size() - 3) == std::string(".vm")) {
        _vm_files.push_back(path);
      }
    }
  }
}