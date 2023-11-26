#pragma once

#include <fstream>

#include "compilation_engine.hpp"

class XMLCompilationEngine : public CompilationEngine {
public:
  XMLCompilationEngine(JackTokenizer& tokenizer, const std::string& dir, const std::string& name) : 
    CompilationEngine(tokenizer, dir, name){}

  void compileClass() override;
  void compileClassVarDec() override;
  void compileSubroutine() override;
  void compileParameterList() override;
  void compileVarDec() override;
  void compileStatements() override;
  void compileDo() override;
  void compileLet() override;
  void compileWhile() override;
  void compileReturn() override;
  void compileIf() override;
  void compileExpression() override;
  void compileTerm() override;
  void compileExpressionList() override;

private:
  void setup();
  std::string _out_file;
  std::ofstream _ofs;
};

void XMLCompilationEngine::setup() {
  _out_file = _dir + "/" + _name + ".xml";
}

void XMLCompilationEngine::compileClass() {

}

void XMLCompilationEngine::compileClassVarDec() {

}

void XMLCompilationEngine::compileSubroutine() {

}

void XMLCompilationEngine::compileParameterList() {

}

void XMLCompilationEngine::compileVarDec() {

}

void XMLCompilationEngine::compileStatements() {

}

void XMLCompilationEngine::compileDo() {

}

void XMLCompilationEngine::compileLet() {

}

void XMLCompilationEngine::compileWhile() {

}

void XMLCompilationEngine::compileReturn() {

}

void XMLCompilationEngine::compileIf() {

}

void XMLCompilationEngine::compileExpression() {

}

void XMLCompilationEngine::compileTerm() {

}

void XMLCompilationEngine::compileExpressionList() {
  
}
