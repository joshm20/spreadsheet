#include "parser.h"

bool
Parser::has (TokenType type)
{
  // This can be written as a single readable boolean expression. No need for a
  // conditional.
  return i < tokens.size () && tokens.at (i).getType () == type;
};

void
Parser::advance ()
{
  i++;
}

std::unique_ptr<Expression>
Parser::parse ()
{
  if (tokens.empty ())
    {
      return std::make_unique<String> ("", 0, 0);
    }

  std::unique_ptr<Expression> exp = block ();
  // Parsing ended prematurely
  // More specifically, the issue is that there are stray, non-grammatical
  // tokens after the expression.
  if (i < tokens.size ())
    {
      std::string text = tokens.at (i).getText ();
      // Turn tokens vector into a debug format with Token strings

      std::string index = std::to_string (tokens.at (i).getStartIndex ());
      throw std::runtime_error ("Syntax error around " + text + " at index "
                                + index);
    }

  return exp;
}

std::unique_ptr<Expression>
Parser::block ()
{
  std::vector<std::unique_ptr<Expression> > statements;
  while (i < tokens.size ())
    {

      std::unique_ptr<Expression> statement = assignment ();
      statements.push_back (std::move (statement));
      while (has (TokenType::NEWLINE) || has (TokenType::SEMICOLON))
        {
          advance ();
        }
      if (has (TokenType::END)
          || has (
              TokenType::ELSE)) // To end If, Else, and For statement blocks
        {
          break;
        }
    }
  return std::make_unique<Block> (std::move (statements), 0, 0);
}

std::unique_ptr<Expression>
Parser::assignment ()
{
  std::unique_ptr<Expression> left = level0 ();
  while (has (TokenType::ASSIGNMENT))
    {
      advance ();
      std::unique_ptr<Expression> right = level0 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<Assignment> (std::move (left), std::move (right),
                                           start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level0 ()
{
  std::unique_ptr<Expression> left = level1 ();
  while (has (TokenType::OR))
    {
      advance ();
      std::unique_ptr<Expression> right = level1 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<Or> (std::move (left), std::move (right),
                                   start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level1 ()
{
  std::unique_ptr<Expression> left = level2 ();
  while (has (TokenType::AND))
    {
      advance ();
      std::unique_ptr<Expression> right = level2 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<And> (std::move (left), std::move (right),
                                    start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level2 ()
{
  std::unique_ptr<Expression> left = level3 ();
  while (has (TokenType::BITOR))
    {
      advance ();
      std::unique_ptr<Expression> right = level3 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<BitOr> (std::move (left), std::move (right),
                                      start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level3 ()
{
  std::unique_ptr<Expression> left = level4 ();
  while (has (TokenType::BITXOR))
    {
      advance ();
      std::unique_ptr<Expression> right = level4 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<BitXor> (std::move (left), std::move (right),
                                       start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level4 ()
{
  std::unique_ptr<Expression> left = level5 ();
  while (has (TokenType::BITAND))
    {
      advance ();
      std::unique_ptr<Expression> right = level5 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<BitAnd> (std::move (left), std::move (right),
                                       start_index, end_index);
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level5 ()
{
  std::unique_ptr<Expression> left = level6 ();
  while (has (TokenType::EQUALS) || has (TokenType::NOTEQUALS))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      std::unique_ptr<Expression> right = level6 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      if (type == TokenType::EQUALS)
        {
          left = std::make_unique<Equals> (std::move (left), std::move (right),
                                           start_index, end_index);
        }
      else // NOTEQUALS
        {
          left = std::make_unique<NotEquals> (
              std::move (left), std::move (right), start_index, end_index);
        }
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level6 ()
{
  std::unique_ptr<Expression> left = level7 ();
  while (has (TokenType::LESSTHAN) || has (TokenType::LESSTHANEQUAL)
         || has (TokenType::GREATERTHAN) || has (TokenType::GREATERTHANEQUAL))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      std::unique_ptr<Expression> right = level7 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      switch (type) // Woah switch ?!
        {
        case TokenType::LESSTHAN:
          left = std::make_unique<LessThan> (
              std::move (left), std::move (right), start_index, end_index);
          break;
        case TokenType::LESSTHANEQUAL:
          left = std::make_unique<LessThanEqual> (
              std::move (left), std::move (right), start_index, end_index);
          break;
        case TokenType::GREATERTHAN:
          left = std::make_unique<GreaterThan> (
              std::move (left), std::move (right), start_index, end_index);
          break;
        case TokenType::GREATERTHANEQUAL:
          left = std::make_unique<GreaterThanEqual> (
              std::move (left), std::move (right), start_index, end_index);
          break;
        default:
          std::string start = std::to_string (tokens.at (i).getStartIndex ());
          std::string end = std::to_string (tokens.at (i).getEndIndex ());
          throw std::runtime_error ("Syntax error at index " + start + " to "
                                    + end);
        }
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level7 ()
{
  std::unique_ptr<Expression> left = level8 ();
  while (has (TokenType::LEFTSHIFT) || has (TokenType::RIGHTSHIFT))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      std::unique_ptr<Expression> right = level8 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      if (type == TokenType::LEFTSHIFT)
        {
          left = std::make_unique<LeftShift> (
              std::move (left), std::move (right), start_index, end_index);
        }
      else
        {
          left = std::make_unique<RightShift> (
              std::move (left), std::move (right), start_index, end_index);
        }
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level8 ()
{
  std::unique_ptr<Expression> left = level9 ();
  while (has (TokenType::PLUS) || has (TokenType::MINUS))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      std::unique_ptr<Expression> right = level9 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      if (type == TokenType::PLUS)
        {
          left = std::make_unique<Add> (std::move (left), std::move (right),
                                        start_index, end_index);
        }
      else
        {
          left = std::make_unique<Subtract> (
              std::move (left), std::move (right), start_index, end_index);
        }
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level9 ()
{
  std::unique_ptr<Expression> left = level10 ();
  while (has (TokenType::MULTIPLY) || has (TokenType::DIVIDE)
         || has (TokenType::MODULO))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      std::unique_ptr<Expression> right = level10 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      switch (type)
        {
        case TokenType::MULTIPLY:
          left = std::make_unique<Multiply> (
              std::move (left), std::move (right), start_index, end_index);
          break;
        case TokenType::DIVIDE:
          left = std::make_unique<Divide> (std::move (left), std::move (right),
                                           start_index, end_index);
          break;
        case TokenType::MODULO:
          left = std::make_unique<Modulo> (std::move (left), std::move (right),
                                           start_index, end_index);
          break;
        default:
          std::string start = std::to_string (tokens.at (i).getStartIndex ());
          std::string end = std::to_string (tokens.at (i).getEndIndex ());
          throw std::runtime_error ("Syntax error at index " + start + " to "
                                    + end);
        }
    }
  return left;
}

std::unique_ptr<Expression>
Parser::level10 () // Right associative
{
  // Exponentiation is right-associative, but this code makes it
  // left-associative. You want to allow further exponents in the right
  // operand. Use recursion rather than descent for the right operand. However,
  // because the recursion is going to pick up further operations, the loop
  // will never run more than once. You won't need a loop after you switch to
  // recursion.
  std::unique_ptr<Expression> left = level11 ();
  if (has (TokenType::EXPONENTIATE))
    {
      advance ();
      std::unique_ptr<Expression> right = level10 ();
      int start_index = left->getStartIndex ();
      int end_index = right->getEndIndex ();
      left = std::make_unique<Exponentiation> (
          std::move (left), std::move (right), start_index, end_index);
    }

  return left;
}

std::unique_ptr<Expression>
Parser::level11 () // Right associative operators
{
  if (has (TokenType::NOT))
    {
      advance ();
      std::unique_ptr<Expression> exp = level11 ();
      return std::make_unique<Not> (std::move (exp), exp->getStartIndex (),
                                    exp->getEndIndex ());
    }
  else if (has (TokenType::MINUS)) // Negation
    {
      advance ();
      std::unique_ptr<Expression> exp = level11 ();
      return std::make_unique<Negation> (
          std::move (exp), exp->getStartIndex (), exp->getEndIndex ());
    }
  else if (has (TokenType::BITNOT))
    {
      advance ();
      std::unique_ptr<Expression> exp = level11 ();
      return std::make_unique<BitNot> (std::move (exp), exp->getStartIndex (),
                                       exp->getEndIndex ());
    }
  else
    {
      return level12 (); // Skip down to the next level
    }
}

std::unique_ptr<Expression>
Parser::level12 ()
{
  std::unique_ptr<Expression> ret = nullptr;
  if (has (TokenType::LEFTPARENTHESIS))
    {
      advance ();
      ret = level0 ();
      if (!has (TokenType::RIGHTPARENTHESIS))
        {
          std::string index = std::to_string (tokens[i].getStartIndex ());
          throw std::runtime_error ("Expected right parenthesis at index "
                                    + index);
        }
    }
  else if (has (TokenType::INTEGER))
    {
      int val = std::stoi (tokens[i].getText ());
      int start_index = tokens[i].getStartIndex ();
      int end_index = tokens[i].getEndIndex ();
      ret = std::make_unique<Integer> (val, start_index, end_index);
    }
  else if (has (TokenType::FLOAT))
    {
      float val = std::stof (tokens[i].getText ());
      int start_index = tokens[i].getStartIndex ();
      int end_index = tokens[i].getEndIndex ();
      ret = std::make_unique<Float> (val, start_index, end_index);
    }
  else if (has (TokenType::BOOLEAN))
    {
      bool val = tokens[i].getText () == "true";
      int start_index = tokens[i].getStartIndex ();
      int end_index = tokens[i].getEndIndex ();
      ret = std::make_unique<Boolean> (val, start_index, end_index);
    }
  else if (has (TokenType::STRING))
    {
      std::string val = tokens[i].getText ();
      int start_index = tokens[i].getStartIndex ();
      int end_index = tokens[i].getEndIndex ();
      ret = std::make_unique<String> (val, start_index, end_index);
    }
  else if (has (TokenType::LEFTBRACKET)) // LValue
    {
      advance (); // Consume left bracket
      std::unique_ptr<Expression> left = level0 ();
      if (!has (TokenType::COMMA))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected comma at index " + index);
        }
      advance (); // Consume comma
      std::unique_ptr<Expression> right = level0 ();
      if (!has (TokenType::RIGHTBRACKET))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected right bracket at index "
                                    + index);
        }
      ret = std::make_unique<LValue> (std::move (left), std::move (right),
                                      left->getStartIndex (),
                                      right->getEndIndex ());
    }
  else if (has (TokenType::HASHTAG)) // RValue
    {
      advance (); // Consume hashtag
      if (has (TokenType::LEFTBRACKET))
        {
          advance (); // Consume left bracket
          std::unique_ptr<Expression> left = level0 ();
          if (!has (TokenType::COMMA))
            {
              std::string index
                  = std::to_string (tokens.at (i).getStartIndex ());
              throw std::runtime_error ("Expected comma at index " + index);
            }
          advance (); // Consume comma
          std::unique_ptr<Expression> right = level0 ();
          if (!has (TokenType::RIGHTBRACKET))
            {
              std::string index
                  = std::to_string (tokens.at (i).getStartIndex ());
              throw std::runtime_error ("Expected right bracket at index "
                                        + index);
            }
          ret = std::make_unique<RValue> (std::move (left), std::move (right),
                                          left->getStartIndex (),
                                          right->getEndIndex ());
        }
      else
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error (
              "Expected left bracket after hashtag at index " + index);
        }
    }
  else if (has (TokenType::VARIABLE))
    { // TODO
      std::string val = tokens.at (i).getText ();
      int start_index = tokens.at (i).getStartIndex ();
      int end_index = tokens.at (i).getEndIndex ();
      // advance ();
      ret = std::make_unique<Variable> (val, start_index, end_index);
    }
  else if (has (TokenType::INTTOFLOAT) || has (TokenType::FLOATTOINT))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      if (!has (TokenType::LEFTPARENTHESIS))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected left parenthesis at index "
                                    + index);
        }
      advance (); // Consume left parenthesis
      std::unique_ptr<Expression> exp = level0 ();
      if (!has (TokenType::RIGHTPARENTHESIS))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected right parenthesis at index "
                                    + index);
        }

      if (type == TokenType::INTTOFLOAT)
        {
          ret = std::make_unique<IntToFloat> (
              std::move (exp), exp->getStartIndex () - 1,
              exp->getEndIndex () + 1); // Wrong indexes?
        }
      else if (type == TokenType::FLOATTOINT)
        {
          ret = std::make_unique<FloatToInt> (std::move (exp),
                                              exp->getStartIndex () - 1,
                                              exp->getEndIndex () + 1);
        }
    }
  else if (has (TokenType::MAX) || has (TokenType::MIN) || has (TokenType::SUM)
           || has (TokenType::MEAN))
    {
      TokenType type = tokens.at (i).getType ();
      advance ();
      if (!has (TokenType::LEFTPARENTHESIS))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected left parenthesis at index "
                                    + index);
        }
      advance (); // Consume left parenthesis
      std::unique_ptr<Expression> left = level0 ();
      if (!has (TokenType::COMMA))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected comma at index " + index);
        }
      advance (); // Consume comma
      std::unique_ptr<Expression> right = level0 ();
      if (!has (TokenType::RIGHTPARENTHESIS))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected right parenthesis at index "
                                    + index);
        }

      if (type == TokenType::SUM)
        {
          ret = std::make_unique<Sum> (std::move (left), std::move (right),
                                       left->getStartIndex (),
                                       right->getEndIndex ());
        }
      else if (type == TokenType::MEAN)
        {
          ret = std::make_unique<Mean> (std::move (left), std::move (right),
                                        left->getStartIndex (),
                                        right->getEndIndex ());
        }
      else if (type == TokenType::MAX)
        {
          ret = std::make_unique<Max> (std::move (left), std::move (right),
                                       left->getStartIndex (),
                                       right->getEndIndex ());
        }
      else if (type == TokenType::MIN)
        {
          ret = std::make_unique<Min> (std::move (left), std::move (right),
                                       left->getStartIndex (),
                                       right->getEndIndex ());
        }
    }
  else if (has (TokenType::IF))
    {
      advance ();
      std::unique_ptr<Expression> condition = level0 ();
      if (!has (TokenType::NEWLINE))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected newline in if statement");
        }
      // Consume NEWLINE
      advance ();

      std::unique_ptr<Expression> block1 = block ();
      std::unique_ptr<Expression> block2 = nullptr;
      if (has (TokenType::ELSE))
        {
          advance ();
          while (has (TokenType::NEWLINE))
            {
              advance ();
            }
          block2 = block ();
        }
      else
        {
          throw std::runtime_error ("Expected ELSE after IF");
        }

      if (!has (TokenType::END))
        throw std::runtime_error ("Expected END after IF-ELSE block");

      ret = std::make_unique<IfExpr> (
          std::move (condition), std::move (block1), std::move (block2),
          condition->getStartIndex (), block2->getEndIndex ());
    }
  else if (has (TokenType::FOR))
    {
      advance ();
      std::unique_ptr<Expression> variable = level0 ();
      if (!has (TokenType::IN))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected 'in' after 'for' variable");
        }
      advance ();
      std::unique_ptr<Expression> left = level0 ();
      if (!has (TokenType::DOTDOT))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected .. at index " + index);
        }
      advance ();
      std::unique_ptr<Expression> right = level0 ();

      if (!has (TokenType::NEWLINE))
        {
          std::string index = std::to_string (tokens.at (i).getStartIndex ());
          throw std::runtime_error ("Expected newline in for statement");
        }

      // Consume NEWLINE
      advance ();

      std::unique_ptr<Expression> loopBlock = block ();

      if (!has (TokenType::END))
        throw std::runtime_error ("Expected END after FOR block");

      ret = std::make_unique<ForExpr> (
          std::move (variable), std::move (left), std::move (right),
          std::move (loopBlock), variable->getStartIndex (),
          loopBlock->getEndIndex ());
    }
  else
    {
      // Should not be reached if lexer works correctly and all token types are
      // handled
      std::string token = tokens.at (i).getText ();
      throw std::runtime_error ("Syntax error on token: \"" + token + "\"");
    }

  advance ();

  return ret;
}
