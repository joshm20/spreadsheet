/* Contains declarations for expressions, primitives,
 * (Arithmetic/Logical/Bitwise/Relational/Casting) operations, lvalues,
 * rvalues, and statistical functions.
 *
 * @author Josh Makela
 * @date 12-17-2024
 */

#ifndef expression_H
#define expression_H
#include "forward_declarations.h"
#include "runtime.h"
#include <memory>
#include <string>
#include <vector>

/*
Expression, BinaryOperation, UnaryOperation, and Primitive are abstractions for
various expressions and operations. Their destructors are handled the same.
 */
class Expression
{
protected:
  int start;
  int end;

public:
  Expression (int start, int end) : start (start), end (end) {}
  // Returns a string representation of the expression
  virtual std::string serialize () = 0;
  // Returns a model Primitive that represents what the string evaluates to
  virtual std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) = 0;

  int
  getStartIndex ()
  {
    return start;
  }

  int
  getEndIndex ()
  {
    return end;
  }

  virtual ~Expression () {};
};

class BinaryOperation : public Expression
{
protected:
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;

public:
  BinaryOperation (std::unique_ptr<Expression> left,
                   std::unique_ptr<Expression> right, int start, int end)
      : Expression (start, end), left (std::move (left)),
        right (std::move (right)) {};
  virtual ~BinaryOperation () {}
};

class UnaryOperation : public Expression
{
protected:
  std::unique_ptr<Expression> exp;

public:
  UnaryOperation (std::unique_ptr<Expression> exp, int start, int end)
      : Expression (start, end), exp (std::move (exp)) {};
  virtual ~UnaryOperation () {}
};

class Primitive : public Expression
{
public:
  Primitive (int start, int end) : Expression (start, end) {};
  ~Primitive () {};
};

//------------------------------- Primitives -------------------------------

class Integer : public Primitive
{
private:
  int val;

public:
  Integer (int val, int start, int end) : Primitive (start, end), val (val) {};
  int getVal ();
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Float : public Primitive
{
private:
  float val;

public:
  Float (float val, int start, int end) : Primitive (start, end), val (val) {};
  float getVal ();
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Boolean : public Primitive
{
private:
  bool val;

public:
  Boolean (bool val, int start, int end)
      : Primitive (start, end), val (val) {};
  bool getVal ();
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class String : public Primitive
{
private:
  std::string val;

public:
  String (std::string val, int start, int end);
  std::string getVal ();
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class CellAddress : public Primitive
{
private:
  int row;
  int col;

public:
  CellAddress (int row, int col, int start, int end);

  int getRow ();
  int getCol ();
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

// -------------------- Arithmetic Operations --------------------

class Add : public BinaryOperation
{

public:
  Add (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
       int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Subtract : public BinaryOperation
{
public:
  Subtract (std::unique_ptr<Expression> left,
            std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Multiply : public BinaryOperation
{

public:
  Multiply (std::unique_ptr<Expression> left,
            std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Divide : public BinaryOperation
{
public:
  Divide (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
          int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Modulo : public BinaryOperation
{
public:
  Modulo (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
          int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Exponentiation : public BinaryOperation
{

public:
  Exponentiation (std::unique_ptr<Expression> left,
                  std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Negation : public UnaryOperation
{

public:
  Negation (std::unique_ptr<Expression> exp, int start, int end)
      : UnaryOperation (std::move (exp), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//---------------------- Logical Operations --------------------------
class And : public BinaryOperation
{

public:
  And (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
       int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Or : public BinaryOperation
{

public:
  Or (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
      int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Not : public UnaryOperation
{
public:
  Not (std::unique_ptr<Expression> exp, int start, int end)
      : UnaryOperation (std::move (exp), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//--------------------------- Cell Values --------------------------
class LValue : public BinaryOperation
{
public:
  LValue (std::unique_ptr<Expression> row, std::unique_ptr<Expression> col,
          int start, int end)
      : BinaryOperation (std::move (row), std::move (col), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class RValue : public BinaryOperation
{

public:
  RValue (std::unique_ptr<Expression> row, std::unique_ptr<Expression> col,
          int start, int end)
      : BinaryOperation (std::move (row), std::move (col), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//--------------------------- Bitwise Operations --------------------------

class BitAnd : public BinaryOperation
{

public:
  BitAnd (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
          int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class BitOr : public BinaryOperation
{

public:
  BitOr (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
         int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class BitXor : public BinaryOperation
{

public:
  BitXor (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
          int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class BitNot : public UnaryOperation
{
public:
  BitNot (std::unique_ptr<Expression> exp, int start, int end)
      : UnaryOperation (std::move (exp), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class LeftShift : public BinaryOperation
{

public:
  LeftShift (std::unique_ptr<Expression> left,
             std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class RightShift : public BinaryOperation
{
public:
  RightShift (std::unique_ptr<Expression> left,
              std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//--------------------- Relational Operations --------------------------

class Equals : public BinaryOperation
{

public:
  Equals (std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
          int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class NotEquals : public BinaryOperation
{

public:
  NotEquals (std::unique_ptr<Expression> left,
             std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class LessThan : public BinaryOperation
{

public:
  LessThan (std::unique_ptr<Expression> left,
            std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class LessThanEqual : public BinaryOperation
{

public:
  LessThanEqual (std::unique_ptr<Expression> left,
                 std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class GreaterThan : public BinaryOperation
{

public:
  GreaterThan (std::unique_ptr<Expression> left,
               std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class GreaterThanEqual : public BinaryOperation
{

public:
  GreaterThanEqual (std::unique_ptr<Expression> left,
                    std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//--------------------------- Casting Operations --------------------------

class FloatToInt : public UnaryOperation
{

public:
  FloatToInt (std::unique_ptr<Expression> exp, int start, int end)
      : UnaryOperation (std::move (exp), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class IntToFloat : public UnaryOperation
{

public:
  IntToFloat (std::unique_ptr<Expression> exp, int start, int end)
      : UnaryOperation (std::move (exp), start, end) {};
  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

//----------------------- Statistical Functions--------------------------

class Max : public BinaryOperation
{

public:
  Max (std::unique_ptr<Expression> topLeft,
       std::unique_ptr<Expression> bottomRight, int start, int end)
      : BinaryOperation (std::move (topLeft), std::move (bottomRight), start,
                         end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Min : public BinaryOperation
{

public:
  Min (std::unique_ptr<Expression> topLeft,
       std::unique_ptr<Expression> bottomRight, int start, int end)
      : BinaryOperation (std::move (topLeft), std::move (bottomRight), start,
                         end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Mean : public BinaryOperation
{

public:
  Mean (std::unique_ptr<Expression> topLeft,
        std::unique_ptr<Expression> bottomRight, int start, int end)
      : BinaryOperation (std::move (topLeft), std::move (bottomRight), start,
                         end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Sum : public BinaryOperation
{

public:
  Sum (std::unique_ptr<Expression> topLeft,
       std::unique_ptr<Expression> bottomRight, int start, int end)
      : BinaryOperation (std::move (topLeft), std::move (bottomRight), start,
                         end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

// --------------------- Blocks, Variables, and Assignments -------------------
class Block : public Expression
{
private:
  std::vector<std::unique_ptr<Expression> > statements;

public:
  Block (std::vector<std::unique_ptr<Expression> > statements, int start,
         int end)
      : Expression (start, end), statements (std::move (statements)) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class Variable : public Expression
{
private:
  std::string name;

public:
  Variable (std::string name, int start, int end)
      : Expression (start, end), name (name) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
  std::string getName ();
};

class Assignment : public BinaryOperation
{
public:
  Assignment (std::unique_ptr<Expression> left,
              std::unique_ptr<Expression> right, int start, int end)
      : BinaryOperation (std::move (left), std::move (right), start, end) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class IfExpr : public Expression
{
private:
  std::unique_ptr<Expression> condition;
  std::unique_ptr<Expression> ifTrue;
  std::unique_ptr<Expression> ifFalse;

public:
  IfExpr (std::unique_ptr<Expression> condition,
          std::unique_ptr<Expression> ifTrue,
          std::unique_ptr<Expression> ifFalse, int start, int end)
      : Expression (start, end), condition (std::move (condition)),
        ifTrue (std::move (ifTrue)), ifFalse (std::move (ifFalse)) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

class ForExpr : public Expression
{
private:
  std::unique_ptr<Expression> variable;
  std::unique_ptr<Expression> left;
  std::unique_ptr<Expression> right;
  std::unique_ptr<Expression> block;

public:
  ForExpr (std::unique_ptr<Expression> variable,
           std::unique_ptr<Expression> left, std::unique_ptr<Expression> right,
           std::unique_ptr<Expression> block, int start, int end)
      : Expression (start, end), variable (std::move (variable)),
        left (std::move (left)), right (std::move (right)),
        block (std::move (block)) {};

  std::string serialize () override;
  std::unique_ptr<Primitive>
  evaluate (std::shared_ptr<Runtime> runtime) override;
};

#endif
