#pragma once

#include <fstream>
#include <algorithm>

#include "log.hpp"
#include "common.hpp"

class CodeWriter {
public:
  CodeWriter(){}
  ~CodeWriter() {
    close();
  }

  bool init(const std::string& asm_file);
  void setFileName(const std::string& fileName);
  bool writeArithmetic(const std::string& command);
  bool writePushPop(CMDType command, const std::string& segment, int index);
  void close();

private:
  void write(const std::string& instruction);
  void writeAdd();
  void writeSub();
  void writeNeg();
  void writeEq();
  void writeGt();
  void writeLt();
  void writeAnd();
  void writeOr();
  void writeNot();

  void writeLabel(const std::string& label_name);

  std::string _asm_file_path{""};
  std::string _symbol_prefix{""};
  std::ofstream _asm_ofs;

  bool _has_true_label{false};
  bool _has_false_label{false};
  int _label_idx{0};
};

bool CodeWriter::init(const std::string& asm_file) {
  _asm_file_path = asm_file;
  if (_asm_ofs.is_open()) {
    return false;
  }
  _asm_ofs.open(_asm_file_path, std::fstream::trunc);
  if (!_asm_ofs.is_open()) {
    return false;
  }
  return true;
}

void CodeWriter::setFileName(const std::string& fileName) {
  _symbol_prefix = fileName;
  auto pos = _symbol_prefix.find_last_of(".vm");
  if (pos != std::string::npos) {
    _symbol_prefix = _symbol_prefix.substr(0, pos);
  }
  for (auto iter = _symbol_prefix.begin(); iter != _symbol_prefix.end(); ++iter) {
    if (*iter == '.') {
      *iter = '_';
    } else if (*iter == '/') {
      *iter = '_';
    }
  }
  LOG(CodeWriter) << "current symbol prefix: " << _symbol_prefix << std::endl;
}

bool CodeWriter::writeArithmetic(const std::string& command) {
  if (command == "add") {
    writeAdd();
    return true;
  }
  if (command == "sub") {
    writeSub();
    return true;
  }
  if (command == "neg") {
    writeNeg();
    return true;
  }
  if (command == "eq") {
    writeEq();
    return true;
  }
  if (command == "gt") {
    writeGt();
    return true;
  }
  if (command == "lt") {
    writeLt();
    return true;
  }
  if (command == "and") {
    writeAnd();
    return true;
  }
  if (command == "or") {
    writeOr();
    return true;
  }
  if (command == "not") {
    writeNot();
    return true;
  }
  LOG(CodeWriter) << "command not support: " << command << std::endl;
  return false;
}

bool CodeWriter::writePushPop(CMDType command, const std::string& segment, int index) {
  if (command == CMDType::C_PUSH) {
    if (segment == s_segment_constant) {
      write("@" + std::to_string(index));
      write("D=A");
    } else if (segment == s_segment_pointer) {
      int ram_idx = POINTER_BASE + index;
      if (ram_idx > POINTER_END) {
        LOG(CodeWriter) << "index out of range" << std::endl;
      } else {
        write("@R" + std::to_string(ram_idx));
        write("D=M");
      }
    } else if (segment == s_segment_tmp) {
      int ram_idx = TMP_BASE + index;
      if (ram_idx > TMP_END) {
        LOG(CodeWriter) << "index out of range" << std::endl;
      } else {
        write("@R" + std::to_string(ram_idx));
        write("D=M");
      }
    } else if (segment == s_segment_static) {
      write("@" + _symbol_prefix + "." + std::to_string(index));
      write("D=M");
    }else if (k_segment_map.count(segment) > 0) {
        write("@" + std::to_string(index));
        write("D=A");
        write("@" + k_segment_map.at(segment));
        write("A=D+M");
        write("D=M");
    } else {
      LOG(CodeWriter) << "unknown segment: " << segment << std::endl;
      return false;
    }

    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");
    return true;
  }

  if (command == CMDType::C_POP) {
    if (segment == s_segment_tmp) {
      int ram_idx = TMP_BASE + index;
      if (ram_idx > TMP_END) {
        LOG(CodeWriter) << "index out of range: " << index;
        return false;
      }
      write("@SP");
      write("M=M-1");
      write("A=M");
      write("D=M");
      write("@R" + std::to_string(ram_idx));
      write("M=D");
    } else if (segment == s_segment_pointer) {
      int ram_idx = POINTER_BASE + index;
      if (ram_idx > POINTER_END) {
        LOG(CodeWriter) << "index out of range: " << index;
        return false;
      }
      write("@SP");
      write("M=M-1");
      write("A=M");
      write("D=M");
      write("@R" + std::to_string(ram_idx));
      write("M=D");
    } else if (segment == s_segment_static) {
      write("@SP");
      write("M=M-1");
      write("A=M");
      write("D=M");
      write("@" + _symbol_prefix + "." + std::to_string(index));
      write("M=D");
    } else if (k_segment_map.count(segment) > 0) {
      write("@SP");
      write("M=M-1");
      
      write("@" + std::to_string(index));
      write("D=A");
      write("@" + k_segment_map.at(segment));
      write("D=D+M");
      write("@R13");
      write("M=D");
      write("@SP");
      write("A=M");
      write("D=M");
      write("@R13");
      write("A=M");
      write("M=D");
    } else {
      LOG(CodeWriter) << "unknown segment: " << std::endl;
      return false;
    }
    return true;
  }
  return false;
}

void CodeWriter::close() {
  if (_asm_ofs.is_open()) {
    _asm_ofs.close();
  }
}

void CodeWriter::write(const std::string& instruction) {
  _asm_ofs << instruction << std::endl;
}

void CodeWriter::writeAdd() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=D+M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");
}

void CodeWriter::writeSub() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M-D");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");
}

void CodeWriter::writeNeg() {
  write("@SP");
  write("A=M-1");
  write("D=-M");
  write("@SP");
  write("A=M-1");
  write("M=D");
}

void CodeWriter::writeEq() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");

  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=D-M");

  std::string true_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string false_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string end_label_name = "LABEL" + std::to_string(++_label_idx);

  write("@" + true_label_name);
  write("D;JEQ");
  write("@" + false_label_name);
  write("0;JMP");

  write("(" + true_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")");
}

void CodeWriter::writeGt() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");

  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M-D");

  std::string true_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string false_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string end_label_name = "LABEL" + std::to_string(++_label_idx);

  write("@" + true_label_name);
  write("D;JGT");

  write("@" + false_label_name);
  write("0;JMP");

  write("(" + true_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")");
}

void CodeWriter::writeLt() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");

  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M-D");

  std::string true_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string false_label_name = "LABEL" + std::to_string(++_label_idx);
  std::string end_label_name = "LABEL" + std::to_string(++_label_idx);

  write("@" + true_label_name);
  write("D;JLT");

  write("@" + false_label_name);
  write("0;JMP");

  write("(" + true_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")");
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")");
}

void CodeWriter::writeAnd() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=D&M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");
}

void CodeWriter::writeOr() {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=D|M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");
}

void CodeWriter::writeNot() {
  write("@SP");
  write("A=M-1");
  write("D=!M");
  write("@SP");
  write("A=M-1");
  write("M=D");
}

void CodeWriter::writeLabel(const std::string& label_name) {
  write("(" + label_name + ")");
  write("@65535");
  write("D=A");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");
}