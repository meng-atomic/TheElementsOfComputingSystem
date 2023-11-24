#pragma once

#include <fstream>
#include <vector>

#include "log.hpp"
#include "common.hpp"

class Parser {
public:
  Parser();
  ~Parser();

  bool init(const std::string& vm_path);
  bool hasMoreCommands();
  bool advance();
  CMDType commandType() const;
  const std::string&  arg1() const;
  int arg2() const;

private:
  bool parse_command();

  std::string _vm_path{""};
  std::ifstream _ifs;
  int _line_no{0};
  std::string _current_cmd{""};
  CMDType _command_type{CMDType::C_UNKNOWN};
  std::string _arg1{""};
  int _arg2{-1};

  const static std::string s_k_comment;
  const static char s_k_white_space;
};

const std::string Parser::s_k_comment = "//";
const char Parser::s_k_white_space = ' ';


Parser::Parser(){
}

Parser::~Parser() {
  if (_ifs.is_open()) {
    _ifs.close();
  }
}

bool Parser::init(const std::string& vm_path) {
  if (vm_path.find_last_of(".vm") == std::string::npos) {
    LOG(Parser) << "error file format: " << vm_path << std::endl;
    return false;
  }
  _vm_path = vm_path;
  return true;
}

bool Parser::hasMoreCommands() {
  if (!_ifs.is_open()) {
    _ifs.open(_vm_path, std::fstream::in);
    if (!_ifs.is_open()) {
      return false;
    }
  }
  if (_ifs.eof()) {
    return false;
  }
  return true;
}

bool Parser::advance() {
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
    }
    // remove white space
    auto start_pos = _current_cmd.find_first_not_of(s_k_white_space);
    if (start_pos == std::string::npos) {
      continue;
    }
    if (start_pos > 0) {
      _current_cmd.erase(0, start_pos - 1);
    }
    auto end_pos = _current_cmd.find_last_not_of(s_k_white_space);
    if (end_pos < _current_cmd.size()) {
      _current_cmd.erase(end_pos + 1, _current_cmd.size() - end_pos - 1);
    }
    // empty line
    if (_current_cmd.empty()) {
      continue;
    }
    break;
  }
  if (!_current_cmd.empty()) {
    LOG(Parser) << "preprocessed command: [" << _line_no << "]: " << _current_cmd << std::endl;
    return parse_command();
  }
  return false;
}

bool Parser::parse_command() {
  _command_type = CMDType::C_UNKNOWN;
  _arg1 = "";
  _arg2 = -1;

  std::vector<std::string> components;
  for (auto iter = _current_cmd.cbegin(); iter < _current_cmd.cend(); ++iter) {
    if (*iter == s_k_white_space) {
      continue;
    }
    auto end_iter = iter;
    while (++end_iter != _current_cmd.end()) {
      if (*end_iter == s_k_white_space) {
        break;
      }
    }
    components.emplace_back(_current_cmd.substr(iter - _current_cmd.cbegin(), end_iter - iter));
    iter = end_iter;
  }

  std::string cmd_type_str{""};
  if (components.size() > 0) {
    cmd_type_str = components.front();
    if (k_cmd_map.count(cmd_type_str) > 0) {
      _command_type = k_cmd_map.at(cmd_type_str);
    }
  } else {
    LOG(Parser) << "error parse command: " << _current_cmd << std::endl;
    return false;
  }
  
  if (_command_type == CMDType::C_ARITHMETIC) {
    _arg1 = cmd_type_str;
  } else {
    if (components.size() > 1) {
      _arg1 = components[1];
    }

    if (components.size() > 2) {
      _arg2 = std::atoi(components[2].c_str());
    }
  }
  
  return true;
}

CMDType Parser::commandType() const {
  return _command_type;
}

const std::string& Parser::arg1() const {
  return _arg1;
}

int Parser::arg2() const {
  return _arg2;
}
