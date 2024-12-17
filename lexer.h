#ifndef lexer_H
#define lexer_H

#include "token.h"
#include <cctype>
#include <iostream>
#include <memory>
#include <vector>

class Lexer
{
private:
  std::string source;
  size_t i;
  std::string token_so_far;
  std::vector<Token> tokens;

public:
  Lexer (std::string source) : source (source), i (0), token_so_far ("") {}
  bool has (char target);
  void capture ();
  void emit_token (TokenType type);
  bool has_alphabetic ();
  bool has_alphanumeric ();
  bool has_digit ();
  bool has_whitespace ();
  bool has_newline ();
  std::unique_ptr<std::vector<Token> > lex ();
};

#endif