#include "token.h"

std::string
Token::getText ()
{
  return text;
}

TokenType
Token::getType ()
{
  return type;
}
int
Token::getStartIndex ()
{
  return start_index;
}

int
Token::getEndIndex ()
{
  return end_index;
}

std::string
Token::getTypeString ()
{
  return token_type_strings[static_cast<int> (type)];
}

// Overload the << operator to print the token
std::ostream &
operator<< (std::ostream &os, Token &token)
{
  os << "[Text: " << token.getText () << "\tType: " << token.getTypeString ()
     << "\tStart Index: " << token.getStartIndex ()
     << "\tEnd Index: " << token.getEndIndex () << "]\n";
  return os;
}