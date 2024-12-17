#ifndef cell_H
#define cell_H
#include "expression.h"
#include "forward_declarations.h"
#include <memory>
#include <string>

// Cell class represents a single cell in the spreadsheet.
// It holds a string of the source code (will be implemented in the future),
// an Expression, and the Primitive that the expression evaluates to.
class Cell
{
private:
  // shared_pointer is for memory that you expect to
  // have many owners. It keeps a reference count that deallocates
  // automatically when the count goes to 0. unique_ptr is for memory that has
  // a single owner. It deallocates automatically when the owner goes out of
  // scope. Most of your dynamic allocations have single owners.

  std::string src;

  // Field for the source abstract syntax tree. This has been changed from a
  // unique_ptr to a shared_ptr, as it needs to be accessible from the grid,
  // and if it was moved out of the cell then problems would arise.
  std::shared_ptr<Expression> exp;
  std::unique_ptr<Primitive>
      primitive; // Field for what the expression evaluates to (a primitive)
  std::string error;

public:
  Cell (std::string src, std::unique_ptr<Expression> exp,
        std::unique_ptr<Primitive> primitive, std::string error);
  std::string getString ();
  std::shared_ptr<Expression> getExpression ();
  std::unique_ptr<Primitive> getPrimitive (std::shared_ptr<Runtime> runtime);
  std::string getError ();
  void setStr (std::string string);
  void setExpression (std::unique_ptr<Expression> expression,
                      std::shared_ptr<Runtime> runtime);
  void setPrimitive (std::unique_ptr<Primitive> prim);
  void setError (std::string error);
};

#endif