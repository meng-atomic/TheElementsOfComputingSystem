#pragma once

#include "macro.hpp"
#include "log.hpp"
#include "parser.hpp"
#include "code.hpp"
#include "symbol_table.hpp"
#include "instruction_writer.hpp"

class Assembler {
public:
  bool init(const std::string& asm_path);
  void assembly();

private:
  void load_symbols();
  void load_variables();
  void generate_instruction();
  std::string _asm_path{""};
  std::string _hack_path{""};
  Parser _parser;
  SymbolTable _symbol_table;
  InstructionWriter _writer;
};

bool Assembler::init(const std::string& asm_path) {
  if (!_parser.init(asm_path)) {
    return false;
  }
  auto pos = asm_path.find_last_of('.');
  auto hack_path = asm_path.substr(0, pos) + ".hack";
  if (!_writer.init(hack_path)) {
    return false;
  }
  _symbol_table.init();
  return true;
}

void Assembler::assembly() {
  load_symbols();
  _parser.reset();
  generate_instruction();
}

void Assembler::load_symbols() {
  int rom_address = 0;
  while(_parser.hasMoreCommands()) {
    _parser.advance();
    auto cmd_type = _parser.commandType();
    if (cmd_type == CMDType::A_COMMAND || cmd_type == CMDType::C_COMMAND) {
      ++rom_address;
      continue;
    }
    if (cmd_type == CMDType::L_COMMAND) {
      _symbol_table.addEntry(_parser.symbol(), rom_address);
    }
  }
  _symbol_table.echo();
}

void Assembler::generate_instruction() {
  int ram_address = RAM_BASE;
  while (_parser.hasMoreCommands()) {
    _parser.advance();
    auto cmd_type = _parser.commandType();
    if (cmd_type == CMDType::A_COMMAND) {
      auto symbol = _parser.symbol();
      if (Code::is_number(symbol)) {
        _writer.write(std::bitset<WIDTH>(std::atoi(symbol.c_str())));
        continue;
      }
      if (!_symbol_table.contains(symbol)) {
        _symbol_table.addEntry(symbol, ram_address++);
      }
      _writer.write(std::bitset<WIDTH>(_symbol_table.getAddress(symbol)));
    }
    if (cmd_type == CMDType::C_COMMAND) {
      std::bitset<WIDTH> instruction(0b1110000000000000);
      instruction |= Code::dest(_parser.dest());
      instruction |= Code::comp(_parser.comp());
      instruction |= Code::jump(_parser.jump());
      _writer.write(instruction);
      continue;
    }
    if (cmd_type == CMDType::L_COMMAND) {
      LOG(Assembler) << "no code generate for L command" << std::endl;
    }
    if (cmd_type == CMDType::UNKNOWN) {
      abort();
    }
  }
}