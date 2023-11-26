#pragma once

#include <fstream>
#include <regex>

#include "log.hpp"
#include "common.hpp"
#include "preprocessor.hpp"

class JackTokenizer {
public:
  JackTokenizer(const std::string& jack_file);
  bool hasMoreTokens();
  bool advance();
  TokenType tokenType();
  Keyword keyword();
  char symbol();
  std::string identifier();
  int intVal();
  std::string stringVal();

  std::string token() const {
    return _token_str;
  }

  std::string current_line() const {
    return _current_line;
  }

private:
  class TokenParser {
  public:
    TokenParser(){}
    void init(const std::string& str) {
      _current = str.begin();
      _end = str.end();
    }
    bool valid() { return _current < _end; }
    bool parse(TokenType& type, std::string& token);

  private:
    std::string::const_iterator _current;
    std::string::const_iterator _end;
  };

  std::string _jack_file;
  std::ifstream _ifs;

  std::string _current_line;
  int _line_no;
  TokenParser _parser;
  std::string _token_str;
  TokenType _type;
};

JackTokenizer::JackTokenizer(const std::string& jack_file) : _jack_file(jack_file) {
}

bool JackTokenizer::hasMoreTokens() {
  if (!_ifs.is_open()) {
    _ifs.open(_jack_file, std::fstream::in);
    if (!_ifs.is_open()) {
      return false;
    }
  }
  if (_parser.valid()) {
    return true;
  }
  if (_ifs.eof()) {
    return false;
  }
  return true;
}

bool JackTokenizer::advance() {
  if (!_parser.valid()) {
    _current_line.clear();
    while (_ifs.good()) {
      _current_line.clear();
      std::getline(_ifs >> std::ws, _current_line, '\n');
      ++_line_no;
      if (Preprocessor::preprocess(_current_line)) {
        break;
      }
    }
    _parser.init(_current_line);
    if (!_parser.valid()) {
      return false;
    }
  }
  return _parser.parse(_type, _token_str);
}

TokenType JackTokenizer::tokenType() {
  return _type;
}

Keyword JackTokenizer::keyword() {
  if (s_keyword_map.count(_token_str) > 0) {
    return s_keyword_map.at(_token_str);
  }
  return Keyword::K_UNKNOWN;
}

char JackTokenizer::symbol() {
  if (_token_str.size() > 0) {
    return _token_str[0];
  }
  return ' ';
}

std::string JackTokenizer::identifier() {
  return _token_str;
}

int JackTokenizer::intVal() {
  return std::atoi(_token_str.c_str());
}

std::string JackTokenizer::stringVal() {
  return _token_str;
}

bool JackTokenizer::TokenParser::parse(TokenType& type, std::string& token) {
  token.clear();
  while (_current < _end && *_current == ' ') {
    ++_current;
  }
  if (!valid()) {
    return false;
  } 
  auto begin = _current;
  // string constant
  if (*begin == '"') {
    if (begin + 1 == _end) {
      return false;
    }
    while (++_current < _end) {
      if (*_current == '"') {
        token = std::string(begin + 1, _current);
        type = TokenType::STRING_CONST;
        ++_current;
        return true;
      } 
    }
    return false;
  }

  if (*begin == '\'') {
    if (begin + 1 == _end) {
      return false;
    }
    while (++_current < _end) {
      if (*_current == '\'') {
        if (_current - begin > 1) {
          token = std::string(begin + 1, _current);
          type = TokenType::INT_CONST;
          ++_current;
          return true;
        }
      }
    }
    return false;
  }

  if (s_symbol_set.count(*_current) > 0) {
    token = std::string(_current, _current + 1);
    type = TokenType::SYMBOL;
    ++_current;
    return true;
  }

  while (++_current < _end) {
    if (*_current == ' ' || s_symbol_set.count(*_current) > 0) {
      token = std::string(begin, _current);
      if (s_keyword_map.count(token) > 0) {
        type = TokenType::KEYWORD;
      } else {
        static std::regex e("\\d+");
        if (std::regex_match(token, e)) {
          type = TokenType::INT_CONST;
        } else {
          if (std::isdigit(token[0])) {
            return false;
          }
          type = TokenType::IDENTIFIER;
        }
      }
      if (*_current == ' ') {
        ++_current;
      }
      return true;
    }
  }

  return false;
}