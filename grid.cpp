#include "grid.h"

// Currently, cells are initialized to have a src of the empty string, a
// nullptr for an expression, and a nullptr for the primitive.
Grid::Grid ()
{

  for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
        {
          std::unique_ptr<Expression> exp
              = std::make_unique<String> ("", -1, -1);
          cells[i][j] = std::make_shared<Cell> ("", std::move (exp),
                                                exp->evaluate (nullptr), "");
        }
    }
};

void
Grid::setCell (int row, int col, std::string src,
               std::unique_ptr<Expression> exp,
               std::shared_ptr<Runtime> runtime, std::string error)
{
  std::shared_ptr<Cell> cell = cells[row][col];

  if (cell != nullptr)
    {
      cell->setPrimitive (exp->evaluate (runtime));
      cell->setExpression (std::move (exp), runtime);
      cell->setStr (src);
      cell->setError (error);
    }
  else
    {
      cells[row][col] = std::make_shared<Cell> ("", std::move (exp),
                                                exp->evaluate (runtime), "");
    }
}

std::unique_ptr<Primitive>
Grid::getValue (CellAddress *address, std::shared_ptr<Runtime> runtime)
{
  if (address->getRow () < 0 || address->getRow () >= rows
      || address->getCol () < 0 || address->getCol () >= cols)
    throw std::runtime_error (
        "Cell address out of range, sorry user, code is wrong.");
  std::shared_ptr<Cell> cell = cells[address->getRow ()][address->getCol ()];
  if (cell == nullptr)
    return nullptr; // Cell is empty
  std::unique_ptr<Primitive> ret = cell->getPrimitive (runtime);
  return ret;
}

std::shared_ptr<Cell>
Grid::getCell (int row, int col)
{
  return cells[row][col];
}

void
Grid::printGrid (std::shared_ptr<Runtime> runtime)
{
  for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
        {
          std::cout << "| ";
          if (cells[i][j] != nullptr)
            {
              std::unique_ptr<Primitive> prim
                  = cells[i][j]->getPrimitive (runtime);
              if (prim != nullptr)
                {
                  std::cout << cells[i][j]->getString () << " = "
                            << prim->serialize () << " |";
                }
            }
          else
            {
              std::cout << "NULL |";
            }
        }
      std::cout << std::endl;
    }
}

void
Grid::updateGrid (std::shared_ptr<Runtime> runtime)
{
  for (int i = 0; i < rows; ++i)
    {
      for (int j = 0; j < cols; ++j)
        {
          std::shared_ptr<Cell> cell = cells[i][j];
          if (cell != nullptr)
            {
              std::shared_ptr<Expression> exp = cell->getExpression ();
              if (exp != nullptr)
                {
                  cell->setPrimitive (exp->evaluate (runtime));
                }
            }
        }
    }
}

Grid::~Grid () {}

// Function to get a reference to the cells array
std::shared_ptr<Cell> **
Grid::getCells ()
{
  return reinterpret_cast<std::shared_ptr<Cell> **> (cells);
}