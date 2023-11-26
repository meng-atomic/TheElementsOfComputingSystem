#pragma once

#include <unordered_set>
#include <unordered_map>
#include <iostream>

enum class TokenType {
  KEYWORD,
  SYMBOL,
  IDENTIFIER,
  INT_CONST,
  STRING_CONST,
};

std::ostream& operator<<(std::ostream& os, TokenType type) {
  switch (type)
  {
  case TokenType::KEYWORD:
    os << "KEYWORD";
    break;
  case TokenType::SYMBOL:
    os << "SYMBOL";
    break;
  case TokenType::IDENTIFIER:
    os << "IDENTIFIER";
    break;
  case TokenType::INT_CONST:
    os << "INT_CONST";
    break;
  case TokenType::STRING_CONST:
    os << "STRING_CONST";
    break;
  default:
    break;
  }
  return os;
}

enum class Keyword {
  K_UNKNOWN,
  K_CLASS,
  K_METHOD,
  K_FUNCTION,
  K_CONSTRUCTOR,
  K_INT,
  K_BOOLEAN,
  K_CHAR,
  K_VOID,
  K_VAR,
  K_STATIC,
  K_FIELD,
  K_LET,
  K_DO,
  K_IF,
  K_ELSE,
  K_WHILE,
  K_RETURN,
  K_TRUE,
  K_FALSE,
  K_NULL,
  K_THIS,
};

const static std::unordered_set<char> s_symbol_set = {
  '{', '}', '(', ')', '[', ']', '.', ',', ';', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~'
};

const static std::unordered_map<std::string, Keyword> s_keyword_map = {
  {"class",  Keyword::K_CLASS},
  {"method",  Keyword::K_METHOD},
  {"function",  Keyword::K_FUNCTION},
  {"constructor",  Keyword::K_CONSTRUCTOR},
  {"int",  Keyword::K_INT},
  {"boolean",  Keyword::K_BOOLEAN},
  {"char",  Keyword::K_CHAR},
  {"void",  Keyword::K_VOID},
  {"var",  Keyword::K_VAR},
  {"static",  Keyword::K_STATIC},
  {"field",  Keyword::K_FIELD},
  {"let",  Keyword::K_LET},
  {"do",  Keyword::K_DO},
  {"if",  Keyword::K_IF},
  {"else",  Keyword::K_ELSE},
  {"while",  Keyword::K_WHILE},
  {"return",  Keyword::K_RETURN},
  {"true",  Keyword::K_TRUE},
  {"false",  Keyword::K_FALSE},
  {"null",  Keyword::K_NULL},
  {"this",  Keyword::K_THIS},
};