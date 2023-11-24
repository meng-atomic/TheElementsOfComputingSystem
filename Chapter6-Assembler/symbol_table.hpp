#pragma once

#include <unordered_map>
#include "macro.hpp"
#include "log.hpp"

class SymbolTable {
public:
  SymbolTable();
  ~SymbolTable();

  void init();
  void addEntry(const std::string& symbol, int address);
  bool contains(const std::string& symbol);
  int getAddress(const std::string& symbol);
  void echo();

private:
  std::unordered_map<std::string, int> _symbol_table;
};

SymbolTable::SymbolTable() {

}

SymbolTable::~SymbolTable() {

}

void SymbolTable::init() {
  addEntry("SP",    0);
  addEntry("LCL",   1);
  addEntry("ARG",   2);
  addEntry("THIS",  3);
  addEntry("THAT",  4);
  for (int i = 0; i < 16; ++i) {
    addEntry("R" + std::to_string(i), i);
  }
  addEntry("SCREEN", 16384);
  addEntry("KBD",    24576);
}

void SymbolTable::addEntry(const std::string& symbol, int address) {
  _symbol_table.insert({symbol, address});
}

bool SymbolTable::contains(const std::string& symbol) {
  return _symbol_table.count(symbol) > 0;
}

int SymbolTable::getAddress(const std::string& symbol) {
  return _symbol_table[symbol];
}

void SymbolTable::echo() {
  for (auto& pair : _symbol_table) {
    LOG(Symbol) << pair.second << "\t" << pair.first << std::endl;
  }
}