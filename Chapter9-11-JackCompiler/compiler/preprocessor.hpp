#pragma once

#include "common.hpp"

#include <string>

const static std::string s_k_comment = "//";
const static char s_k_white_space = ' ';

class Preprocessor {
public:
  static bool preprocess(std::string& content);
};

bool Preprocessor::preprocess(std::string& content) {
  auto newline_pos = content.find_last_of((char(13)));
  if (newline_pos != std::string::npos) {
    content.erase(newline_pos);
  }
  // remove comment
  auto pos = content.find_first_of(s_k_comment);
  if (pos != std::string::npos) {
    content.erase(pos);
  }
  // remove white space
  auto start_pos = content.find_first_not_of(s_k_white_space);
  if (start_pos != std::string::npos && start_pos > 0) {
    content.erase(0, start_pos - 1);
  }
  auto end_pos = content.find_last_not_of(s_k_white_space);
  if (end_pos < content.size()) {
    content.erase(end_pos + 1, content.size() - end_pos - 1);
  }
  if (content.empty()) {
    return false;
  }
  return true;
}