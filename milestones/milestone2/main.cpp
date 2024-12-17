#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int
main ()
{

  std::shared_ptr<Grid> grid = std::make_shared<Grid> ();
  std::shared_ptr<Runtime> runtime = std::make_shared<Runtime> (grid);

  /*
    Arithmetic: (5 + 2) * 3 % 4
    Rvalue lookup and shift: #[0, 0] + 3
    Rvalue lookup and comparison: #[1 - 1, 0] < #[1 * 1, 1]
    Logic and comparison: (5 > 3) && !(2 > 8)
    Sum: 1 + sum([0, 0], [2, 1])
    Casting: float(10) / 4.0
    Exponentiation: 2 ** 3 ** 2
    Negation and bitwise and: 45 & ---(1 + 3)
  */

  std::string arithmetic = "(5 + 2) * 3 % 4";
  std::string rvalue = "#[0, 0] + 3";
  std::string comparison = "-#[1 - 1, 0] < #[1 * 1, 1]";
  std::string logic = "(5 > 3) && !(2 > 8)";
  std::string sum = "1 + sum([0, 0], [2, 1])";
  std::string casting = "float(10.0) / 4";
  std::string exponentiation = "2 ** 3 ** 2";
  std::string negation = "45 & ---(1 + 3)";
  // Known issues: Adding a number "2 ** 3 7 ** 2" does not throw an error, but
  // stops after the 3. Adding an unmatched parenthesis or bracket or comma
  // stops parsing weirdly. Not sure of the fix yet.
  // Slightly patched with check if there is more tokens.

  // Print tokens
  // for (size_t i = 0; i < tokens->size (); i++)
  //   {
  //     std::cout << tokens->at (i);
  //   }

  Lexer lexer = Lexer (arithmetic);
  std::unique_ptr<std::vector<Token> > tokens = lexer.lex ();
  for (size_t i = 0; i < tokens->size (); i++)
    {
      std::cout << tokens->at (i);
    }
  Parser parser = Parser (*tokens);
  grid->setCell (0, 0, arithmetic, parser.parse (), runtime);

  lexer = Lexer (rvalue);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (0, 1, rvalue, parser.parse (), runtime);

  // lexer = Lexer (comparison);
  // parser = Parser (*(lexer.lex ()));
  // grid->setCell (0, 2, comparison, parser.parse (), runtime);

  lexer = Lexer (logic);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (0, 3, logic, parser.parse (), runtime);

  lexer = Lexer (sum);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (0, 4, sum, parser.parse (), runtime);

  lexer = Lexer (casting);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (0, 5, casting, parser.parse (), runtime);

  lexer = Lexer (exponentiation);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (0, 6, exponentiation, parser.parse (), runtime);
  lexer = Lexer (exponentiation);
  parser = Parser (*(lexer.lex ()));
  std::cout << parser.parse ()->serialize () << std::endl;

  lexer = Lexer (negation);
  parser = Parser (*(lexer.lex ()));
  grid->setCell (1, 0, negation, parser.parse (), runtime);

  grid->updateGrid (runtime);
  grid->printGrid (runtime);
}