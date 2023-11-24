#pragma once

#include <bitset>
#include <unordered_map>
#include "macro.hpp"

class Code {
public:
  static std::bitset<WIDTH> dest(const std::string& dest_mnemonic);
  static std::bitset<WIDTH> comp(const std::string& comp_mnemonic);
  static std::bitset<WIDTH> jump(const std::string& jump_mnemonic);
  static bool is_number(const std::string& symbol);
  static std::bitset<WIDTH> symbol(const std::string& sym);
};

std::bitset<WIDTH> Code::dest(const std::string& dest_mnemonic) {
  const static size_t s_k_dest_M = 3;
  const static size_t s_k_dest_D = 4;
  const static size_t s_k_dest_A = 5;
  const static char s_k_dest_M_MNEMONIC = 'M';
  const static char s_k_dest_D_MNEMONIC = 'D';
  const static char s_k_dest_A_MNEMONIC = 'A';
  std::bitset<WIDTH> dest(0);
  if (dest_mnemonic.empty()) {
    return dest;
  }
  if (dest_mnemonic.find(s_k_dest_M_MNEMONIC) != std::string::npos) {
    dest.set(s_k_dest_M);
  }
  if (dest_mnemonic.find(s_k_dest_D_MNEMONIC) != std::string::npos) {
    dest.set(s_k_dest_D);
  }
  if (dest_mnemonic.find(s_k_dest_A_MNEMONIC) != std::string::npos) {
    dest.set(s_k_dest_A);
  }
  return dest;
}

std::bitset<WIDTH> Code::comp(const std::string& comp_mnemonic) {
  const static std::unordered_map<std::string, std::bitset<WIDTH>> s_comp_map = {
    {"0",   std::bitset<WIDTH>{0b0101010}},
    {"1",   std::bitset<WIDTH>{0b0111111}},
    {"-1",  std::bitset<WIDTH>{0b0111010}},
    {"D",   std::bitset<WIDTH>{0b0001100}},
    {"A",   std::bitset<WIDTH>{0b0110000}},
    {"!D",  std::bitset<WIDTH>{0b0001101}},
    {"!A",  std::bitset<WIDTH>{0b0110001}},
    {"-D",  std::bitset<WIDTH>{0b0001111}},
    {"-A",  std::bitset<WIDTH>{0b0110011}},
    {"D+1", std::bitset<WIDTH>{0b0011111}},
    {"1+D", std::bitset<WIDTH>{0b0011111}},
    {"A+1", std::bitset<WIDTH>{0b0110111}},
    {"1+A", std::bitset<WIDTH>{0b0110111}},
    {"D-1", std::bitset<WIDTH>{0b0001110}},
    {"A-1", std::bitset<WIDTH>{0b0110010}},
    {"D+A", std::bitset<WIDTH>{0b0000010}},
    {"A+D", std::bitset<WIDTH>{0b0000010}},
    {"D-A", std::bitset<WIDTH>{0b0010011}},
    {"A-D", std::bitset<WIDTH>{0b0000111}},
    {"D&A", std::bitset<WIDTH>{0b0000000}},
    {"A&D", std::bitset<WIDTH>{0b0000000}},
    {"D|A", std::bitset<WIDTH>{0b0010101}},
    {"A|D", std::bitset<WIDTH>{0b0010101}},
    {"M",   std::bitset<WIDTH>{0b1110000}},
    {"!M",  std::bitset<WIDTH>{0b1110001}},
    {"-M",  std::bitset<WIDTH>{0b1110011}},
    {"M+1", std::bitset<WIDTH>{0b1110111}},
    {"1+M", std::bitset<WIDTH>{0b1110111}},
    {"M-1", std::bitset<WIDTH>{0b1110010}},
    {"D+M", std::bitset<WIDTH>{0b1000010}},
    {"M+D", std::bitset<WIDTH>{0b1000010}},
    {"D-M", std::bitset<WIDTH>{0b1010011}},
    {"M-D", std::bitset<WIDTH>{0b1000111}},
    {"D&M", std::bitset<WIDTH>{0b1000000}},
    {"M&A", std::bitset<WIDTH>{0b1000000}},
    {"D|M", std::bitset<WIDTH>{0b1010101}},
    {"M|A", std::bitset<WIDTH>{0b1010101}}
  };
  if (s_comp_map.count(comp_mnemonic) > 0) {
    return s_comp_map.at(comp_mnemonic) << 6;
  }
  LOG(Code) << "unknown comp mnemonic: " << comp_mnemonic << ", " << comp_mnemonic.size() << std::endl;
  return std::bitset<WIDTH>(0);
}

std::bitset<WIDTH> Code::jump(const std::string& jump_mnemonic) {
  const static std::unordered_map<std::string, std::bitset<WIDTH>> s_jump_map = {
    {"null",  std::bitset<WIDTH>{0b000}},
    {"JGT",   std::bitset<WIDTH>{0b001}},
    {"JEQ",   std::bitset<WIDTH>{0b010}},
    {"JGE",   std::bitset<WIDTH>{0b011}},
    {"JLT",   std::bitset<WIDTH>{0b100}},
    {"JNE",   std::bitset<WIDTH>{0b101}},
    {"JLE",   std::bitset<WIDTH>{0b110}},
    {"JMP",   std::bitset<WIDTH>{0b111}},
  };
  if (s_jump_map.count(jump_mnemonic) > 0) {
    return s_jump_map.at(jump_mnemonic);
  }
  LOG(Code) << "unknown jump mnemonic: " << jump_mnemonic;
  return std::bitset<WIDTH>(0);
}

bool Code::is_number(const std::string& symbol) {
  if (symbol.empty()) {
    return false;
  }
  bool is_digit = true;
  std::for_each(symbol.cbegin(), symbol.cend(), [&is_digit](const char& c){
    is_digit &= std::isdigit(c);
  });
  return is_digit;
}

std::bitset<WIDTH> Code::symbol(const std::string& sym) {
  bool is_digit = true;
  std::for_each(sym.cbegin(), sym.cend(), [&is_digit](const char& c){
    is_digit &= std::isdigit(c);
  });
  if (is_digit) {
    int address = std::atoi(sym.c_str());
    if (address > MAX_ADDRESS) {
      LOG(Code) << "address out of range: " << sym << std::endl;
    }
    return std::bitset<WIDTH>(std::atoi(sym.c_str()));
  }
  return std::bitset<WIDTH>(0);
}