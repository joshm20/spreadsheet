#include "lexer.h"
#include <iostream>
#include <memory>

bool
Lexer::has (char target)
{
  return source[i] == target;
}

void
Lexer::capture ()
{
  token_so_far += source[i];
  i++;
}

void
Lexer::emit_token (TokenType type)
{
  tokens.emplace_back (token_so_far, type, i - token_so_far.size (), i - 1);
  token_so_far.clear ();
}

bool
Lexer::has_alphabetic ()
{
  if (i >= source.size ())
    return false;
  return isalpha (source[i]);
}

bool
Lexer::has_alphanumeric ()
{
  if (i >= source.size ())
    return false;
  return isalnum (source[i]);
}

bool
Lexer::has_digit ()
{
  if (i >= source.size ())
    return false;
  return isdigit (source[i]);
}

bool
Lexer::has_whitespace ()
{
  if (i >= source.size ())
    return false;
  bool isspace_not_newline = isspace (source[i]) && source[i] != '\n';
  return isspace_not_newline;
}

bool
Lexer::has_newline ()
{
  if (i >= source.size ())
    return false;
  return source[i] == '\n';
}

std::unique_ptr<std::vector<Token> >
Lexer::lex ()
{
  while (i < source.size ())
    {
      if (has_whitespace ())
        {
          i++;
        }
      else if (has_newline ())
        {
          capture ();
          emit_token (TokenType::NEWLINE);
        }
      else if (has_digit ())
        {
          while (has_digit ())
            {
              capture ();
            }
          if (has ('.'))
            {
              capture ();
              while (has_digit ())
                {
                  capture ();
                }
              emit_token (TokenType::FLOAT);
            }
          else
            {
              emit_token (TokenType::INTEGER);
            }
        }
      else if (has_alphabetic ())
        {
          while (has_alphanumeric ())
            {
              capture ();
            }
          if (token_so_far == "true" || token_so_far == "false")
            {
              emit_token (TokenType::BOOLEAN);
            }
          else if (token_so_far == "sum")
            {
              emit_token (TokenType::SUM);
            }
          else if (token_so_far == "mean")
            {
              emit_token (TokenType::MEAN);
            }
          else if (token_so_far == "min")
            {
              emit_token (TokenType::MIN);
            }
          else if (token_so_far == "max")
            {
              emit_token (TokenType::MAX);
            }
          else if (token_so_far == "float")
            {
              emit_token (TokenType::INTTOFLOAT);
            }
          else if (token_so_far == "int")
            {
              emit_token (TokenType::FLOATTOINT);
            }
          else if (token_so_far == "if")
            {
              emit_token (TokenType::IF);
            }
          else if (token_so_far == "else")
            {
              emit_token (TokenType::ELSE);
            }
          else if (token_so_far == "for")
            {
              emit_token (TokenType::FOR);
            }
          else if (token_so_far == "in")
            {
              emit_token (TokenType::IN);
            }
          else if (token_so_far == "end")
            {
              emit_token (TokenType::END);
            }
          else
            {
              emit_token (TokenType::VARIABLE);
            }
        }
      else if (has ('+'))
        {
          capture ();
          emit_token (TokenType::PLUS);
        }
      else if (has ('-'))
        {
          capture ();
          emit_token (TokenType::MINUS);
        }
      else if (has ('*'))
        {
          capture ();
          if (has ('*'))
            {
              capture ();
              emit_token (TokenType::EXPONENTIATE);
            }
          else
            {
              emit_token (TokenType::MULTIPLY);
            }
        }
      else if (has ('/'))
        {
          capture ();
          emit_token (TokenType::DIVIDE);
        }
      else if (has ('%'))
        {
          capture ();
          emit_token (TokenType::MODULO);
        }
      else if (has ('^')) // Delete
        {
          capture ();
          emit_token (TokenType::BITXOR);
        }
      else if (has ('&'))
        {
          capture ();
          if (has ('&'))
            {
              capture ();
              emit_token (TokenType::AND);
            }
          else
            {
              emit_token (TokenType::BITAND);
            }
        }
      else if (has ('|'))
        {
          capture ();
          if (has ('|'))
            {
              capture ();
              emit_token (TokenType::OR);
            }
          else
            {
              emit_token (TokenType::BITOR);
            }
        }
      else if (has ('~'))
        {
          capture ();
          emit_token (TokenType::BITNOT);
        }
      else if (has ('<'))
        {
          capture ();
          if (has ('='))
            {
              capture ();
              emit_token (TokenType::LESSTHANEQUAL);
            }
          else if (has ('<'))
            {
              capture ();
              emit_token (TokenType::LEFTSHIFT);
            }
          else
            {
              emit_token (TokenType::LESSTHAN);
            }
        }
      else if (has ('>'))
        {
          capture ();
          if (has ('='))
            {
              capture ();
              emit_token (TokenType::GREATERTHANEQUAL);
            }
          else if (has ('>'))
            {
              capture ();
              emit_token (TokenType::RIGHTSHIFT);
            }
          else
            {
              emit_token (TokenType::GREATERTHAN);
            }
        }
      else if (has ('='))
        {
          capture ();
          if (has ('='))
            {
              capture ();
              emit_token (TokenType::EQUALS);
            }
          else
            { // Variable assignment
              emit_token (TokenType::ASSIGNMENT);
            }
        }
      else if (has ('!'))
        {
          capture ();
          if (has ('='))
            {
              capture ();
              emit_token (TokenType::NOTEQUALS);
            }
          else
            {
              emit_token (TokenType::NOT);
            }
        }
      else if (has ('('))
        {
          capture ();
          emit_token (TokenType::LEFTPARENTHESIS);
        }
      else if (has (')'))
        {
          capture ();
          emit_token (TokenType::RIGHTPARENTHESIS);
        }
      else if (has ('['))
        {
          capture ();
          emit_token (TokenType::LEFTBRACKET);
        }
      else if (has (']'))
        {
          capture ();
          emit_token (TokenType::RIGHTBRACKET);
        }
      else if (has ('#'))
        {
          capture ();
          emit_token (TokenType::HASHTAG);
        }
      else if (has (','))
        {
          capture ();
          emit_token (TokenType::COMMA);
        }
      else if (has ('\"'))
        {
          capture ();
          while (!has ('\"'))
            {
              capture ();
            }
          if (i >= source.size ())
            {
              throw std::runtime_error ("Unterminated string");
            }
          capture ();
          token_so_far = token_so_far.substr (
              1,
              token_so_far.size () - 2); // Trim quotes, maybe change so that
                                         // there is an advance() method?
          emit_token (TokenType::STRING);
        }
      else if (has ('.'))
        {
          capture ();
          if (has ('.'))
            {
              capture ();
              emit_token (TokenType::DOTDOT);
            }
          else
            {
              throw std::runtime_error (
                  "Unrecognized character \'.\', check context");
            }
        }
      else if (has (';'))
        {
          capture ();
          emit_token (TokenType::SEMICOLON);
        }
      else if (has (':'))
        {
          capture ();
          emit_token (TokenType::COLON);
        }
      else
        {
          throw std::runtime_error ("Unrecognized character \'"
                                    + std::string (1, source[i])
                                    + "\' at index = " + std::to_string (i));
        }
    }
  return std::make_unique<std::vector<Token> > (tokens);
}
