#ifndef token_H
#define token_H

#include <iostream>

enum class TokenType;

// Represents a token created by the lexer. A token stores the source text
// that produces the token, the type of the token, and the starting and
// ending indexes that it is at.
class Token
{

private:
  std::string text;
  TokenType type;
  int start_index;
  int end_index;

public:
  Token (std::string text, TokenType type, int start_index, int end_index)
      : text (text), type (type), start_index (start_index),
        end_index (end_index)
  {
  }
  std::string getText ();
  TokenType getType ();
  int getStartIndex ();
  int getEndIndex ();
  std::string getTypeString ();

  // A friend method! I haven't seen one of these in a long time.
  friend std::ostream &operator<< (std::ostream &os, Token &token);
};

enum class TokenType
{
  INTEGER,
  FLOAT,
  BOOLEAN,
  STRING,

  PLUS,
  MINUS, // Serves as both unary and binary
  MULTIPLY,
  DIVIDE,
  MODULO,
  NEGATE, // Not removing this to not mess up array indexing, may need in
          // future

  EXPONENTIATE,

  AND,
  OR,
  NOT,
  BITAND,
  BITOR,
  BITXOR,
  BITNOT,
  LEFTSHIFT,
  RIGHTSHIFT,

  LESSTHAN,
  LESSTHANEQUAL,
  GREATERTHAN,
  GREATERTHANEQUAL,
  EQUALS,
  NOTEQUALS,

  INTTOFLOAT,
  FLOATTOINT,

  SUM,
  MEAN,
  MIN,
  MAX,

  LEFTPARENTHESIS,
  RIGHTPARENTHESIS,
  LEFTBRACKET,
  RIGHTBRACKET,
  HASHTAG,
  COMMA,
  NEWLINE,
  SEMICOLON,
  COLON,
  VARIABLE,
  IF,
  ELSE,
  END,

  FOR,
  IN,
  DOTDOT,

  ASSIGNMENT,

};

// Convert the token type to a string
inline const char *token_type_strings[] = {
  "INTEGER",
  "FLOAT",
  "BOOLEAN",
  "STRING",

  "PLUS",
  "MINUS",
  "MULTIPLY",
  "DIVIDE",
  "MODULO",
  "NEGATE",

  "EXPONENTIATE",

  "AND",
  "OR",
  "NOT",
  "BITAND",
  "BITOR",
  "BITXOR",
  "BITNOT",
  "LEFTSHIFT",
  "RIGHTSHIFT",

  "LESSTHAN",
  "LESSTHANEQUAL",
  "GREATERTHAN",
  "GREATERTHANEQUAL",
  "EQUALS",
  "NOTEQUALS",

  "INTTOFLOAT",
  "FLOATTOINT",

  "SUM",
  "MEAN",
  "MIN",
  "MAX",

  "LEFTPARENTHESIS",
  "RIGHTPARENTHESIS",
  "LEFTBRACKET",
  "RIGHTBRACKET",
  "HASHTAG",
  "COMMA",
  "VARIABLE",
};

#endif
