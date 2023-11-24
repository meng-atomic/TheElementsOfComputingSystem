#pragma once

#include <string>
#include <unordered_map>
#include <iostream>

enum class CMDType {
  C_ARITHMETIC,
  C_PUSH,
  C_POP,
  C_LABEL,
  C_GOTO,
  C_IF,
  C_FUNCTION,
  C_RETURN,
  C_CALL,
  C_UNKNOWN,
};

std::ostream& operator<<(std::ostream& os, CMDType cmd) {
  switch (cmd) {
    case CMDType::C_ARITHMETIC:
      os << "C_ARITHMETIC";
      break;
    case CMDType::C_PUSH:
      os << "C_PUSH";
      break;
    case CMDType::C_POP:
      os << "C_POP";
      break;
    case CMDType::C_LABEL:
      os << "C_LABEL";
      break;
    case CMDType::C_GOTO:
      os << "C_GOTO";
      break;
    case CMDType::C_IF:
      os << "C_IF";
      break;
    case CMDType::C_FUNCTION:
      os << "C_FUNCTION";
      break;
    case CMDType::C_RETURN:
      os << "C_RETURN";
      break;
    case CMDType::C_CALL:
      os << "C_CALL";
      break;
    case CMDType::C_UNKNOWN:
      os << "C_UNKNOWN";
      break;
  }
  return os;
}

const static std::string s_cmd_add = "add";
const static std::string s_cmd_sub = "sub";
const static std::string s_cmd_neg = "neg";
const static std::string s_cmd_eq = "eq";
const static std::string s_cmd_gt = "gt";
const static std::string s_cmd_lt = "lt";
const static std::string s_cmd_and = "and";
const static std::string s_cmd_or = "or";
const static std::string s_cmd_not = "not";
const static std::string s_cmd_push = "push";
const static std::string s_cmd_pop = "pop";

using CMDMap = std::unordered_map<std::string, CMDType>;
const static CMDMap k_cmd_map = {
  {s_cmd_add,     CMDType::C_ARITHMETIC},
  {s_cmd_sub,     CMDType::C_ARITHMETIC},
  {s_cmd_neg,     CMDType::C_ARITHMETIC},
  {s_cmd_eq,      CMDType::C_ARITHMETIC},
  {s_cmd_gt,      CMDType::C_ARITHMETIC},
  {s_cmd_lt,      CMDType::C_ARITHMETIC},
  {s_cmd_and,     CMDType::C_ARITHMETIC},
  {s_cmd_or,      CMDType::C_ARITHMETIC},
  {s_cmd_not,     CMDType::C_ARITHMETIC},
  {s_cmd_push,    CMDType::C_PUSH},
  {s_cmd_pop,     CMDType::C_POP},
};

const static size_t POINTER_BASE = 3;
const static size_t POINTER_END = 4;
const static size_t TMP_BASE = 5;
const static size_t TMP_END = 12;
const static size_t GENERAL_BASE = 13;
const static size_t GENERAL_END = 15;
const static size_t STATIC_SEGMENT_BASE = 16;
const static size_t STATIC_SEGMENT_END = 255;
const static size_t STACK_SEGMENT_BASE = 256;
const static size_t STACK_SEGMENT_END = 2047;
const static size_t HEAP_SEGMENT_BASE = 2048;
const static size_t HEAD_SEGMENT_END = 16483;
const static size_t MEMORY_MAPPED_IO_BASE = 16384;
const static size_t MEMORY_MAPPED_IO_END = 24575;

const static std::string s_segment_pointer = "pointer";
const static std::string s_segment_tmp = "temp";
const static std::string s_segment_constant = "constant";
const static std::string s_segment_static = "static";
const static int s_pointer_segment_base = 3;
const static int s_tmp_segment_base = 5;

using SegmentMap = std::unordered_map<std::string, std::string>;
const static SegmentMap k_segment_map = {
  {"argument", "ARG"},
  {"local", "LCL"},
  {"this", "THIS"},
  {"that", "THAT"},
};