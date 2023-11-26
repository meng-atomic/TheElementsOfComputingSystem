#pragma once

#include <vector>
#include <string>
#include <filesystem>

#include "common.hpp"
#include "jack_tokenizer.hpp"
#include "xml_compilation_engine.hpp"
#include "utils.hpp"

class JackAnalyzer {
public:
  JackAnalyzer(const std::string& path);
  bool compile();

private:
  std::vector<std::string> _jack_files;
};

JackAnalyzer::JackAnalyzer(const std::string& path) {
  get_all_files(path, ".jack", _jack_files);
}

bool JackAnalyzer::compile() {
  for (auto& jack_file : _jack_files) {
    std::string dir = "";
    std::string name = "";
    std::string ext = "";
    get_file_name(jack_file, dir, name, ext);
    JackTokenizer tokenzier(jack_file);
    XMLCompilationEngine compiler(tokenzier, dir, name);
    compiler.compileClass();
  }
  return true;
}
