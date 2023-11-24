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

  bool init(const std::string& asm_name);
  bool setFileName(const std::string& fileName);
  bool writeInit();
  bool writeLabel(const std::string& label);
  bool writeGoto(const std::string& label);
  bool writeIf(const std::string& label);
  bool writeCall(const std::string& name, int numArgs);
  bool writeReturn();
  bool writeFunction(const std::string& name, int numLocals);
  bool writeArithmetic(const std::string& command);
  bool writePushPop(CMDType command, const std::string& segment, int index);
  void close();

private:
  void write(const std::string& instruction, int indent = 4);
  void writeAdd();
  void writeSub();
  void writeNeg();
  void writeEq();
  void writeGt();
  void writeLt();
  void writeAnd();
  void writeOr();
  void writeNot();

  std::string _asm_file_path{""};
  std::string _symbol_prefix{""};
  std::ofstream _asm_ofs;

  bool _has_true_label{false};
  bool _has_false_label{false};
  int _label_idx{0};
};

bool CodeWriter::init(const std::string& asm_name) {
  _asm_file_path = asm_name + ".asm";
  if (_asm_ofs.is_open()) {
    return false;
  }
  _asm_ofs.open(_asm_file_path, std::fstream::trunc);
  if (!_asm_ofs.is_open()) {
    return false;
  }
  return writeInit();
}

bool CodeWriter::setFileName(const std::string& fileName) {
  _symbol_prefix = fileName;
  auto pos = _symbol_prefix.find_last_of(".vm");
  if (pos != std::string::npos) {
    _symbol_prefix = _symbol_prefix.substr(0, pos - 2);
    pos = _symbol_prefix.find_last_of('/');
    if (pos != std::string::npos) {
      _symbol_prefix = _symbol_prefix.substr(pos + 1);
    }
    LOG(CodeWriter) << "current symbol prefix: " << _symbol_prefix << std::endl;
    return true;
  }
  return false;
}

bool CodeWriter::writeInit() {
  write("@256");
  write("D=A");
  write("@SP");
  write("M=D");

  writeCall(s_sys_init_function_name, 0);
  return true;
}

bool CodeWriter::writeLabel(const std::string& label) {
  write("(" + _symbol_prefix + "." + label + ")", 0);
  return true;
}

bool CodeWriter::writeGoto(const std::string& label) {
  write("@" + _symbol_prefix + "." + label);
  write("0;JMP");
  return true;
}

bool CodeWriter::writeIf(const std::string& label) {
  write("@SP");
  write("M=M-1");
  write("A=M");
  write("D=M");
  write("@" + _symbol_prefix + "." + label);
  write("D;JNE");
  return true;
}

bool CodeWriter::writeCall(const std::string& name, int numArgs) {
  std::string return_label = "LABEL_RET_" + std::to_string(++_label_idx);
  write("@" + return_label);
  write("D=A");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");

  write("@LCL");
  write("D=M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");

  write("@ARG");
  write("D=M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");

  write("@THIS");
  write("D=M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");

  write("@THAT");
  write("D=M");
  write("@SP");
  write("A=M");
  write("M=D");
  write("@SP");
  write("M=M+1");

  write("@SP");
  write("D=M");
  write("@5");
  write("D=D-A");
  write("@" + std::to_string(numArgs));
  write("D=D-A");
  write("@ARG");
  write("M=D");

  write("@SP");
  write("D=M");
  write("@LCL");
  write("M=D");

  write("@" + name);
  write("0;JMP");

  write("(" + return_label + ")", 0);
  return true;
}

bool CodeWriter::writeReturn() {
  // save return address to R13
  write("@LCL");
  write("D=M");
  write("@5");
  write("A=D-A");
  write("D=M");
  write("@R13");
  write("M=D");
  // reposition return value to ARG[0]
  writePushPop(CMDType::C_POP, "argument", 0);
  // restore SP
  write("@ARG");
  write("D=M");
  write("@SP");
  write("M=D+1");
  // restore segments
  write("@LCL");
  write("A=M-1");
  write("D=M");
  write("@THAT");
  write("M=D");

  write("@LCL");
  write("D=M");
  write("@2");
  write("A=D-A");
  write("D=M");
  write("@THIS");
  write("M=D");

  write("@LCL");
  write("D=M");
  write("@3");
  write("A=D-A");
  write("D=M");
  write("@ARG");
  write("M=D");

  write("@LCL");
  write("D=M");
  write("@4");
  write("A=D-A");
  write("D=M");
  write("@LCL");
  write("M=D");
  // return
  write("@R13");
  write("A=M");
  write("0;JMP");
  return true;
}

bool CodeWriter::writeFunction(const std::string& name, int numLocals) {
  write("(" + name + ")", 0);
  for (int i = 0; i < numLocals; ++i) {
    write("@0");
    write("D=A");
    write("@SP");
    write("A=M");
    write("M=D");
    write("@SP");
    write("M=M+1");
  }
  return true;
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
      write("@" + std::to_string(index));
      write("D=A");
      write("@" + k_segment_map.at(segment));
      write("D=D+M");
      write("@SP");
      write("A=M");
      write("M=D");
      write("@SP");
      write("A=M-1");
      write("D=M");
      write("@SP");
      write("A=M");
      write("A=M");
      write("M=D");
      write("@SP");
      write("M=M-1");
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

void CodeWriter::write(const std::string& instruction, int indent) {
  _asm_ofs << std::string(indent, ' ') << instruction << std::endl;
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

  write("(" + true_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")", 0);
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

  write("(" + true_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")", 0);
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

  write("(" + true_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=-1");
  write("@SP");
  write("M=M+1");

  write("@" + end_label_name);
  write("0;JMP");

  write("(" + false_label_name + ")", 0);
  write("@SP");
  write("A=M");
  write("M=0");
  write("@SP");
  write("M=M+1");

  write("(" + end_label_name + ")", 0);
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