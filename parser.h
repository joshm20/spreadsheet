#ifndef parser_H
#define parser_H

#include "expression.h"
#include "token.h"
#include <memory>
#include <stack>
#include <string>
#include <vector>

class Parser
{
private:
  std::vector<Token> tokens;
  size_t i;

public:
  Parser (std::vector<Token> tokens) : tokens (tokens), i (0) {}

  bool has (TokenType type);
  void advance ();

  // See grammar.txt for the grammar
  std::unique_ptr<Expression> parse ();
  std::unique_ptr<Expression> block ();
  std::unique_ptr<Expression> assignment ();
  std::unique_ptr<Expression> level0 ();
  std::unique_ptr<Expression> level1 ();
  std::unique_ptr<Expression> level2 ();
  std::unique_ptr<Expression> level3 ();
  std::unique_ptr<Expression> level4 ();
  std::unique_ptr<Expression> level5 ();
  std::unique_ptr<Expression> level6 ();
  std::unique_ptr<Expression> level7 ();
  std::unique_ptr<Expression> level8 ();
  std::unique_ptr<Expression> level9 ();
  std::unique_ptr<Expression> level10 ();
  std::unique_ptr<Expression> level11 ();
  std::unique_ptr<Expression> level12 ();
};

#endif