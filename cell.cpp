#include "cell.h"
#include <memory>
// Cell class implementation

Cell::Cell (std::string src, std::unique_ptr<Expression> exp,
            std::unique_ptr<Primitive> primitive, std::string error)
    : src (src), exp (std::move (exp)), primitive (std::move (primitive)),
      error ("") {};

std::string
Cell::getString ()
{
  return src;
}

std::shared_ptr<Expression>
Cell::getExpression ()
{
  return exp;
}

std::unique_ptr<Primitive>
Cell::getPrimitive (std::shared_ptr<Runtime> runtime)
{
  if (primitive == nullptr)
    {
      return nullptr;
    }
  // This allows the getPrimitive to return a seperate, new primitive rather
  // than a pointer to the one held by Cell
  return primitive->evaluate (runtime);
}

std::string
Cell::getError ()
{
  return error;
}

void
Cell::setStr (std::string string)
{
  src = string;
}

void
Cell::setExpression (std::unique_ptr<Expression> expression,
                     std::shared_ptr<Runtime> runtime)
{
  exp = std::move (expression);
}

void
Cell::setPrimitive (std::unique_ptr<Primitive> prim)
{
  primitive = std::move (prim);
}

void
Cell::setError (std::string error)
{
  this->error = error;
}