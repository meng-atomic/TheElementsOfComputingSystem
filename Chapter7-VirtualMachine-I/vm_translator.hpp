#pragma once

#include <filesystem>

#include "common.hpp"
#include "parser.hpp"
#include "code_writer.hpp"

class VMTranslator {
public:
  VMTranslator(const std::string& vm_dir);
  void translate();

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
  auto asm_path = real_vm_dir + "/" + _dir_name + ".asm";
  _writer.init(asm_path);

  get_all_vm_files(real_vm_dir);
  for (auto& path : _vm_files) {
    LOG(Translator) << path << std::endl;
  }
}

void VMTranslator::translate() {
  for (auto& vm_file : _vm_files) {
    Parser parser;
    parser.init(vm_file);
    _writer.setFileName(vm_file);
    while (parser.hasMoreCommands()) {
      if (parser.advance()) {
        CMDType cmd_type = parser.commandType();
        if (cmd_type == CMDType::C_PUSH || cmd_type == CMDType::C_POP) {
          _writer.writePushPop(cmd_type, parser.arg1(), parser.arg2());
        } else if (cmd_type == CMDType::C_ARITHMETIC) {
          _writer.writeArithmetic(parser.arg1());
        }
      }
    }   
  }
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