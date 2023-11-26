#pragma once

#include <fstream>

#include "log.hpp"
#include "common.hpp"

class JackTokenizer;

class CompilationEngine {
public:
  CompilationEngine(JackTokenizer& tokenizer, const std::string& dir, const std::string& name) : 
    _tokenizer(tokenizer), _dir(dir), _name(name){};

  virtual void compileClass(){}
  virtual void compileClassVarDec(){}
  virtual void compileSubroutine(){}
  virtual void compileParameterList(){}
  virtual void compileVarDec(){}
  virtual void compileStatements(){}
  virtual void compileDo(){}
  virtual void compileLet(){}
  virtual void compileWhile(){}
  virtual void compileReturn(){}
  virtual void compileIf(){}
  virtual void compileExpression(){}
  virtual void compileTerm(){}
  virtual void compileExpressionList(){}

protected:
  JackTokenizer& _tokenizer;
  std::string _dir;
  std::string _name;
};