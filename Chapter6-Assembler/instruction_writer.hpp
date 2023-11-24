#pragma once

#include <string>
#include <bitset>
#include <fstream>

#include "log.hpp"
#include "macro.hpp"


class InstructionWriter {
public:
  ~InstructionWriter();
  bool init(const std::string& path);
  void write(const std::bitset<WIDTH>& instruction);
  void release();

private:
  std::ofstream _ofs;
};

InstructionWriter::~InstructionWriter() {
  release();
}

bool InstructionWriter::init(const std::string& path) {
  if (_ofs.is_open()) {
    return false;
  }
  _ofs.open(path, std::fstream::trunc);
  if (!_ofs.is_open()) {
    return false;
  }
  return true;
}

void InstructionWriter::write(const std::bitset<WIDTH>& instruction) {
  _ofs << instruction.to_string() << std::endl;
}

void InstructionWriter::release() {
  if (_ofs.is_open()) {
    _ofs.close();
  }
}
