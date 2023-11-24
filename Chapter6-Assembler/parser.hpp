#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include "log.hpp"

enum class CMDType {
  A_COMMAND,
  C_COMMAND,
  L_COMMAND,
  UNKNOWN,
};

class Parser {
public:
  Parser();
  ~Parser();

  bool init(const std::string& path);
  void reset();
  void release();
  bool hasMoreCommands() const;
  void advance();
  CMDType commandType() const;
  std::string symbol() const;
  std::string dest() const;
  std::string comp() const;
  std::string jump() const;
  const std::string& current_cmd() const {
    return _current_cmd;
  }

private:
  void clear_fields();
  void parse_command();
  std::string _path{""};
  std::ifstream _ifs;
  size_t _line_no{0};
  std::string _current_cmd;
  CMDType _command_type{CMDType::UNKNOWN};
  std::string _symbol{""};
  std::string _dest{""};
  std::string _comp{""};
  std::string _jump{""};

  const static std::string s_k_comment;
  const static char s_k_white_space;
  const static char s_k_left_bracket;
  const static char s_k_right_bracket;
  const static char s_k_assign;
  const static char s_k_semicolon;
};

const std::string Parser::s_k_comment = "//";

const char Parser::s_k_white_space = ' ';
const char Parser::s_k_left_bracket = '(';
const char Parser::s_k_right_bracket = ')';
const char Parser::s_k_assign = '=';
const char Parser::s_k_semicolon = ';';

Parser::Parser() {

}

Parser::~Parser() {
  release();
}

bool Parser::init(const std::string& path) {
  if (_ifs.is_open()) {
    return false;
  }
  _ifs.open(path, std::fstream::in);
  if (!_ifs.is_open()) {
    return false;
  }
  _path = path;
  return true;
}

void Parser::reset() {
  release();
  init(_path);
  _line_no = 0;
  _current_cmd.clear();
  clear_fields();
}

void Parser::release() {
  if (_ifs.is_open()) {
    _ifs.close();
  }
}

bool Parser::hasMoreCommands() const {
  if (_ifs.eof()) {
    return false;
  }
  return true;
}

void Parser::advance() {
  _current_cmd.clear();
  while (hasMoreCommands()) {
    std::getline(_ifs >> std::ws, _current_cmd, '\n');
    ++_line_no;
    auto newline_pos = _current_cmd.find_last_of((char(13)));
    if (newline_pos != std::string::npos) {
      _current_cmd.erase(newline_pos);
    }

    // remove comment
    auto pos = _current_cmd.find_first_of(s_k_comment);
    if (pos != std::string::npos) {
      _current_cmd.erase(pos);
      // LOG(Parser) << "erase comment: " << _current_cmd << std::endl;
    }
    // remove white space
    for (auto iter = _current_cmd.begin(); iter != _current_cmd.end();) {
      if (*iter == s_k_white_space) {
        iter = _current_cmd.erase(iter);
      } else {
        ++iter;
      }
    }
    // // LOG(Parser) << "erase all white space: " << _current_cmd << std::endl;

    // empty line
    if (_current_cmd.empty()) {
      continue;
    }
    break;
  }
  if (!_current_cmd.empty()) {
    LOG(Parser) << "preprocessed command: [" << _line_no << "]: " << _current_cmd << std::endl;
    parse_command();
  }
}

CMDType Parser::commandType() const {
  return _command_type;
}

std::string Parser::symbol() const {
  return _symbol;
}

std::string Parser::dest() const {
  return _dest;
}

std::string Parser::comp() const {
  return _comp;
}

std::string Parser::jump() const {
  return _jump;
}

void Parser::clear_fields() {
  _command_type = CMDType::UNKNOWN;
  _symbol = "";
  _dest = "null";
  _comp = "";
  _jump = "null";
}

void Parser::parse_command() {
  clear_fields();
  _command_type = CMDType::UNKNOWN;
  if (_current_cmd.front() == '@') {
    _command_type = CMDType::A_COMMAND;
    if (_current_cmd.size() > 1) {
      _symbol = _current_cmd.substr(1);
    } else {
      // LOG(Parser) << "failed parser A-instruction symbol: " << _current_cmd;
    }
    return;
  }
  auto left_bracket_pos = _current_cmd.find(s_k_left_bracket);
  if (left_bracket_pos != std::string::npos) {
    auto right_bracket_pos = _current_cmd.find(s_k_right_bracket);
    if (right_bracket_pos != std::string::npos && (right_bracket_pos - left_bracket_pos) > 1) {
      _command_type = CMDType::L_COMMAND;
      _symbol = _current_cmd.substr(left_bracket_pos + 1, right_bracket_pos - left_bracket_pos - 1);
      return;
    } else {
      // LOG(Parser) << "parse error at line[" << _line_no << "]: error bracket" << std::endl;
    }
  }
  _command_type = CMDType::C_COMMAND;
  auto assign_pos = _current_cmd.find(s_k_assign);
  auto semi_pos = _current_cmd.find(s_k_semicolon);
  if (assign_pos == std::string::npos && semi_pos == std::string::npos) {
    // LOG(Parser) << "parse error at line[" << _line_no << "]: no dest nor jump" << std::endl;
    return;
  }
  if (assign_pos == 0 || semi_pos == _current_cmd.size() - 1
      || semi_pos == 0 || assign_pos == _current_cmd.size() - 1) {
    // LOG(Parser) << "parse error at line[" << _line_no << "]: syntax error" << std::endl;
    return;
  }
  if (assign_pos != std::string::npos && semi_pos != std::string::npos) {
    if (assign_pos > semi_pos) {
      // LOG(Parser) << "parse error at line[" << _line_no << "]: syntax error" << std::endl;
      return;
    }
    _dest = _current_cmd.substr(0, assign_pos);
    _comp = _current_cmd.substr(assign_pos + 1, semi_pos - assign_pos - 1);
    _jump = _current_cmd.substr(semi_pos + 1);
    return;
  }
  
  if (assign_pos != std::string::npos) {
    _dest = _current_cmd.substr(0, assign_pos);
    _comp = _current_cmd.substr(assign_pos + 1);
    return;
  }

  if (semi_pos != std::string::npos) {
    _comp = _current_cmd.substr(0, semi_pos);
    _jump = _current_cmd.substr(semi_pos + 1);
  }
}
