#pragma once

#include <vector>
#include <stack>
#include <map>

#include "common.hpp"
#include "log.hpp"
#include "jack_tokenizer.hpp"

enum class NodeType {
  NONE,
  CLASS,
  CLASS_VAR_DEC,
  TYPE,
  SUBROUTINE_DEC,
  PARAMETER_LIST,
  SUBROUTINE_BODY,
  VAR_DEC,
  CLASS_NAME,
  SUBROUTINE_NAME,
  VAR_NAME,
  LET_STATEMENT,
  IF_STATEMENT,
  WHILE_STATEMENT,
  DO_STATEMENT,
  RETURN_STATEMENT,
  EXPRESSION,
};

std::ostream& operator<<(std::ostream& os, NodeType type) {
  switch (type)
  {
  case NodeType::NONE:
    os << "NONE";
    break;
  case NodeType::WHILE_STATEMENT:
    os << "WHILE_STATEMENT";
    break;
  case NodeType::EXPRESSION:
    os << "EXPRESSION";
    break;
  
  default:
    break;
  }
  return os;
}

struct Node {
  Node(){}
  ~Node() {
    for (auto child : children) {
      if (child != nullptr) {
        delete child;
      }
    }
  }

  void add_chile(Node* node) {
    if (node == nullptr) {
      return;
    }
    node->parent = this;
    children.push_back(node);
  }

  void reset_expect_symbol() {
    expect_symbol = ' ';
  }

  NodeType type{NodeType::NONE};
  TokenInfo token_info;
  bool terminal{false};
  std::stack<std::string> stack;
  char expect_symbol = ' ';

  Node* parent;
  std::vector<Node*> children;
};

std::ostream& operator<<(std::ostream& os, Node* node) {
  if (node == nullptr) {
    return os;
  }
  os << node->type << ":" << node->token_info.token_str;
  return os;
}

class Parser {
public:
  Parser() : _parse_tree(new Node()), _alive_node(_parse_tree){}
  ~Parser() {
    if (_parse_tree) {
      delete _parse_tree;
    }
  }

  void add_token(const TokenInfo& token_info);
  void print_parse_tree();

private:
  void travel(Node* node, std::map<int, std::vector<Node*>>& node_map, int level = 0);
  void parse_class(const TokenInfo& token_info);
  void parse_while_statement(const TokenInfo& token_info);
  void parse_expression(const TokenInfo& token_info);

  Node* _parse_tree;
  Node* _alive_node;
  std::stack<NodeType> _parse_stack; // for non-terminals
};

void Parser::add_token(const TokenInfo& token_info) {
  if (_parse_stack.empty()) {
    if (token_info.type == TokenType::KEYWORD) {
      if (token_info.keyword == Keyword::K_WHILE) {
        parse_while_statement(token_info);
      }
    }
  } else {
    if (_alive_node->type == NodeType::WHILE_STATEMENT) {
      parse_while_statement(token_info);
    } else if (_alive_node->type == NodeType::EXPRESSION) {
      parse_expression(token_info);
    }
  }
}

void Parser::parse_while_statement(const TokenInfo& token_info) {
  if (_alive_node->type != NodeType::WHILE_STATEMENT) {
    Node* node = new Node();
    node->terminal = false;
    node->type = NodeType::WHILE_STATEMENT;
    node->expect_symbol = '(';

    Node* tnode = new Node();
    tnode->terminal = true;
    tnode->token_info = token_info;
    tnode->type = NodeType::NONE;
    node->add_chile(tnode);
    _alive_node->add_chile(node);

    _alive_node = node;
    _parse_stack.push(NodeType::WHILE_STATEMENT);
  } else {
    if (token_info.symbol == _alive_node->expect_symbol) {
      Node* node = new Node();
      node->type = NodeType::NONE;
      node->token_info = token_info;
      node->terminal = true;
      _alive_node->add_chile(node);

      // expect expression
      Node* enode = new Node();
      enode->type = NodeType::EXPRESSION;
      enode->terminal = false;
      _alive_node->add_chile(enode);
      _alive_node = enode;
      _parse_stack.push(NodeType::EXPRESSION);
      return;
    }
  }
}

void Parser::print_parse_tree() {
  std::map<int, std::vector<Node*>> node_map;
  travel(_parse_tree, node_map);
  for (auto& [level, vec] : node_map) {
    std::cout << level << std::endl;
    for (auto node : vec) {
      std::cout << node << " | ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void Parser::travel(Node* node, std::map<int, std::vector<Node*>>& node_map, int level) {
  node_map[level].push_back(node);
  for (auto child : node->children) {
    travel(child, node_map, level + 1);
  }
}

void Parser::parse_expression(const TokenInfo& token_info) {
  if (token_info.type == TokenType::SYMBOL) {
    if (token_info.symbol == ')') {
      _alive_node = _alive_node->parent;
      add_token(token_info);
      return;
    }
  }
}