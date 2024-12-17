/* Contains implementation for all classes declared in expression.h
  All classes implement a serialize method and an evaluate method. Some have
 getters for their values. Most
 *
 * @author Josh Makela
 * @date 12-17-2024
 */

#include "expression.h"
#include <cmath>
#include <format>
#include <memory>

//--------------- Primitives -------------------

// ---------------------Integer
std::string
Integer::serialize ()
{
  return std::to_string (val);
}

std::unique_ptr<Primitive>
Integer::evaluate (std::shared_ptr<Runtime> runtime)
{
  return std::make_unique<Integer> (
      val, -1,
      -1); // I considered using default arguments for start and end params,
           // but I figure the IDE checking would help catch more misuses if I
           // was declaring these outside evaluate statement
}

int
Integer::getVal ()
{
  return val;
}

// --------------------Float
std::string
Float::serialize ()
{
  return std::format ("{:.2f}", val);
}

std::unique_ptr<Primitive>
Float::evaluate (std::shared_ptr<Runtime> runtime)
{
  return std::make_unique<Float> (val, start, end);
}

float
Float::getVal ()
{
  return val;
}

// ----------------- Boolean
std::string
Boolean::serialize ()
{
  return val ? "true" : "false";
}

std::unique_ptr<Primitive>
Boolean::evaluate (std::shared_ptr<Runtime> runtime)
{
  return std::make_unique<Boolean> (val, start, end);
}

bool
Boolean::getVal ()
{
  return val;
}

//-------------------- String

String::String (std::string val, int start, int end)
    : Primitive (start, end), val (val) {};

std::unique_ptr<Primitive>
String::evaluate (std::shared_ptr<Runtime> runtime)
{
  return std::make_unique<String> (val, start, end);
}

std::string
String::serialize ()
{
  return std::format ("{}", val);
}

std::string
String::getVal ()
{
  return val;
}

// // ------------------ Cell Adresss
CellAddress::CellAddress (int row, int col, int start, int end)
    : Primitive (start, end), row (row), col (col) {};

int
CellAddress::getRow ()
{
  return row;
}

int
CellAddress::getCol ()
{
  return col;
}

std::string
CellAddress::serialize ()
{
  return std::format ("[{}, {}]", row, col);
}

std::unique_ptr<Primitive>
CellAddress::evaluate (std::shared_ptr<Runtime> runtime)
{
  return std::make_unique<CellAddress> (row, col, start, end);
}

// // -------------- Arithmetic Operations --------------------

// ---------------- Add
// Supports adding Integers to Integers, Floats to Floats, Integers to Floats,
// and Strings to Strings

std::string
Add::serialize ()
{
  std::string ret
      = std::format ("({} + {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Add::evaluate (std::shared_ptr<Runtime> runtime)
{

  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);
  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer)) // Integer/Integer case
    {
      ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              + dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1); // Placeholders?
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Float)) // Float/Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              + dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Integer)
           && typeid (*rightprim) == typeid (Float)) // Integer/Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              + dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Integer)) // Float/Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              + dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (String)
           && typeid (*rightprim) == typeid (String)) // String case
    {
      ret = std::make_unique<String> (
          dynamic_cast<String &> (*leftprim).getVal ()
              + dynamic_cast<String &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Add operation");
    }

  return ret;
}

// ---------------- Subtract
// Supports subtracting Integers from Integers and Floats from Floats

std::string
Subtract::serialize ()
{
  std::string ret
      = std::format ("({} - {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Subtract::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;
  // Check both are the same type

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer)) // Integer case
    {
      ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              - dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Float)) // Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              - dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Integer)) // Float/Integer case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              - dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Integer)
           && typeid (*rightprim) == typeid (Float)) // Integer/Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              - dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Subtract operation");
    }

  return ret;
}

// ---------------- Multiply
// Supports multiplying Integers with Integers and Floats with Floats

std::string
Multiply::serialize ()
{
  std::string ret
      = std::format ("({} * {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Multiply::evaluate (std::shared_ptr<Runtime> runtime)
{

  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer)) // Integer case
    {
      ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              * dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Float)) // Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              * dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Integer)) // Float/Integer case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              * dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Integer)
           && typeid (*rightprim) == typeid (Float)) // Integer/Float case
    {
      ret = std::make_unique<Float> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              * dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Multiply operation");
    }

  return ret;
}

//-------------------- Divide
// Supports dividing Integers by Integers and Floats by Floats

std::string
Divide::serialize ()
{
  std::string ret
      = std::format ("({} / {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Divide::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    { // Integer case, C++ defines integer division as truncation toward
      // zero, so this works well enough.
      if (dynamic_cast<Integer &> (*rightprim).getVal () == 0)
        {

          throw std::runtime_error ("Division by zero error");
        }
      ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              / dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Float))
    { // Float case
      if (dynamic_cast<Float &> (*rightprim).getVal () == 0.0f)
        {
          throw std::runtime_error ("Division by zero error");
        }
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              / dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)
           && typeid (*rightprim) == typeid (Integer))
    { // Float and Integer case
      if (dynamic_cast<Integer &> (*rightprim).getVal () == 0)
        {
          throw std::runtime_error ("Division by zero error");
        }
      ret = std::make_unique<Float> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              / dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Integer)
           && typeid (*rightprim) == typeid (Float))
    { // Integer and Float case
      if (dynamic_cast<Float &> (*rightprim).getVal () == 0.0f)
        {
          throw std::runtime_error ("Division by zero error");
        }
      ret = std::make_unique<Float> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              / dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Divide operation");
    }

  return ret;
}

//--------------- Modulo
// Supports modulo operation for Integers
std::string
Modulo::serialize ()
{
  std::string ret
      = std::format ("({} % {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Modulo::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer)) // Integer case
    {
      if (dynamic_cast<Integer &> (*rightprim).getVal () == 0)
        {

          throw std::runtime_error ("Modulo by zero error");
        }
      ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              % dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Modulo operation");
    }

  return ret;
}

// -------------- Exponentiation
// Supports exponentiation for Integer by Integers and Floats by Floats
std::string
Exponentiation::serialize ()
{
  std::string ret
      = std::format ("({} ** {})", left->serialize (), right->serialize ());
  return ret;
}

std::unique_ptr<Primitive>
Exponentiation::evaluate (std::shared_ptr<Runtime> runtime)
{

  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);

  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;
  if (typeid (*leftprim) != typeid (*rightprim))
    {

      throw std::runtime_error ("Type mismatch in Exponentiation operation");
    }

  if (typeid (*leftprim) == typeid (Integer)) // Integer case
    {
      ret = std::make_unique<Integer> (
          std::pow (dynamic_cast<Integer &> (*leftprim).getVal (),
                    dynamic_cast<Integer &> (*rightprim).getVal ()),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float)) // Float case
    {
      ret = std::make_unique<Float> (
          std::pow (dynamic_cast<Float &> (*leftprim).getVal (),
                    dynamic_cast<Float &> (*rightprim).getVal ()),
          -1, -1);
      return ret;
    }
  else
    {
      throw std::runtime_error (
          "Unsupported types for Exponentiation operation");
    }

  return ret;
}

// -------------- Negation
// Supports negation for Integers and Floats

std::string
Negation::serialize ()
{
  return std::format ("(-({}))", exp->serialize ());
}

std::unique_ptr<Primitive>
Negation::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> prim = exp->evaluate (runtime);

  std::unique_ptr<Primitive> ret;
  if (typeid (*prim) == typeid (Integer)) // Integer case
    {
      ret = std::make_unique<Integer> (
          -dynamic_cast<Integer &> (*prim).getVal (), -1, -1);
    }
  else if (typeid (*prim) == typeid (Float)) // Float case
    {
      ret = std::make_unique<Float> (-dynamic_cast<Float &> (*prim).getVal (),
                                     -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported type for Negation operation");
    }

  return ret;
}

// -------------- Logical Operations --------------------

// --------------- And
// Supports logical AND for Boolean values
std::string
And::serialize ()
{
  return std::format ("({} && {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
And::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Boolean))
    {
      // Short if left is false
      if (dynamic_cast<Boolean &> (*leftprim).getVal () == false)
        {
          std::unique_ptr<Primitive> ret
              = std::make_unique<Boolean> (false, -1, -1);
          return ret;
        }

      if (typeid (*rightprim) == typeid (Boolean))
        {
          std::unique_ptr<Primitive> ret = std::make_unique<Boolean> (
              dynamic_cast<Boolean &> (*leftprim).getVal ()
                  && dynamic_cast<Boolean &> (*rightprim).getVal (),
              -1, -1);
          return ret;
        }
      else
        {
          throw std::runtime_error ("Unsupported type for And operation");
        }
    }
  else
    {

      throw std::runtime_error ("Unsupported type for And operation");
    }
}

// --------------- Or
// Supports logical OR for Boolean values
std::string
Or::serialize ()
{
  return std::format ("({} || {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
Or::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Boolean))
    {
      // Short if left is true
      if (dynamic_cast<Boolean &> (*leftprim).getVal () == true)
        {
          std::unique_ptr<Primitive> ret
              = std::make_unique<Boolean> (true, -1, -1);
          return ret;
        }
      if (typeid (*rightprim) == typeid (Boolean))
        {
          std::unique_ptr<Primitive> ret = std::make_unique<Boolean> (
              dynamic_cast<Boolean &> (*leftprim).getVal ()
                  && dynamic_cast<Boolean &> (*rightprim).getVal (),
              -1, -1);
          return ret;
        }
      else
        {
          throw std::runtime_error ("Unsupported type for Or operation");
        }
    }
  else
    {

      throw std::runtime_error ("Unsupported type for Or operation");
    }
}

// -------------- Not
// Supports logical NOT for Boolean values
std::string
Not::serialize ()
{
  return std::format ("!({})", exp->serialize ());
}

std::unique_ptr<Primitive>
Not::evaluate (std::shared_ptr<Runtime> runtime)
{

  std::unique_ptr<Primitive> prim = exp->evaluate (runtime);
  if (typeid (*prim) == typeid (Boolean))
    {

      std::unique_ptr<Primitive> ret = std::make_unique<Boolean> (
          !dynamic_cast<Boolean &> (*prim).getVal (), -1, -1);

      return ret;
    }
  else
    {

      throw std::runtime_error ("Unsupported types for Not operation");
    }
}

//--------------------------- Cell Values --------------------------
// --------------- LValue
// LValue represents a cell address in the spreadsheet. The difference is that
// LValue can take expressions for parameters. However, these
// parameters MUST evaluate to Integer primitives.
std::string
LValue::serialize ()
{
  return std::format ("[{}, {}]", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
LValue::evaluate (std::shared_ptr<Runtime> runtime)
{
  // Should be Integers
  std::unique_ptr<Primitive> rowprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> colprim = right->evaluate (runtime);

  // Spot on. This is how lvalues are evaluated.
  if (typeid (*rowprim) == typeid (Integer)
      && typeid (*colprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<CellAddress> (
          dynamic_cast<Integer &> (*rowprim).getVal (),
          dynamic_cast<Integer &> (*colprim).getVal (), -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for LValue");
    }
}

// ------------- RValue
// RValue represents a cell value in the spreadsheet.
std::string
RValue::serialize ()
{
  return std::format ("#[{}, {}]", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
RValue::evaluate (std::shared_ptr<Runtime> runtime)
{
  // Should be Integers

  std::unique_ptr<Primitive> rowVal = left->evaluate (runtime);
  std::unique_ptr<Primitive> colVal = right->evaluate (runtime);

  if (typeid (*rowVal) == typeid (Integer)
      && typeid (*colVal) == typeid (Integer))
    {
      CellAddress address
          = CellAddress (dynamic_cast<Integer &> (*rowVal).getVal (),
                         dynamic_cast<Integer &> (*colVal).getVal (), -1,
                         -1); // -1 is unimportant I think
      std::unique_ptr<Primitive> cellprim
          = (runtime->getCell (&address, runtime));

      return cellprim;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for RValue");
    }
}

//------------------------- Bitwise Operations --------------------------
// Bitwise Operators should have Integer values
// ---------------- BitAnd
// Supports bitwise AND for Integer values
std::string
BitAnd::serialize ()
{
  return std::format ("({} & {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
BitAnd::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              & dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for BitAnd operation");
    }
}

// ---------------- BitOr
// Supports bitwise OR for Integer values

std::string
BitOr::serialize ()
{
  return std::format ("({} | {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
BitOr::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              | dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for BitOr operation");
    }
}

// ---------------- BitXor
// Supports bitwise XOR for Integer values
// Style Choice: Using ^^ for XOR and ^ for exponentiation
std::string
BitXor::serialize ()
{
  return std::format ("({} ^^ {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
BitXor::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              ^ dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for BitXor operation");
    }
}

// ---------------- BitNot
// Supports bitwise NOT for Integer values
std::string
BitNot::serialize ()
{
  return std::format ("~({})", exp->serialize ());
}

std::unique_ptr<Primitive>
BitNot::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> prim = exp->evaluate (runtime);
  std::unique_ptr<Primitive> ret;

  if (typeid (*prim) == typeid (Integer))
    {
      ret = std::make_unique<Integer> (
          ~dynamic_cast<Integer &> (*prim).getVal (), -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for BitNot operation");
    }
}

// ---------------- LeftShift
// Supports left shift for Integer values

std::string
LeftShift::serialize ()
{
  return std::format ("({} << {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
LeftShift::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              << dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for LeftShift operation");
    }
}

// ---------------- RightShift
// Supports right shift for Integer values

std::string
RightShift::serialize ()
{
  return std::format ("({} >> {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
RightShift::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  if (typeid (*leftprim) == typeid (Integer)
      && typeid (*rightprim) == typeid (Integer))
    {
      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              >> dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);

      return ret;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for RightShift operation");
    }
}

//--------------------- Relational Operations --------------------------

// ---------------- Equals
// Supports equality check for Integer, Float, Boolean, and String types

std::string
Equals::serialize ()
{
  return std::format ("({} == {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
Equals::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in Equals operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              == dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              == dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Boolean))
    { // Boolean Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Boolean &> (*leftprim).getVal ()
              == dynamic_cast<Boolean &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (String))
    { // String Case, works because == is the same as .compare for std::string
      ret = std::make_unique<Boolean> (
          dynamic_cast<String &> (*leftprim).getVal ()
              == dynamic_cast<String &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for Equals operation");
    }

  return ret;
}

// ---------------- NotEquals
// Supports inequality check for Integer, Float, Boolean, and String
// types

std::string
NotEquals::serialize ()
{
  return std::format ("({} != {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
NotEquals::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in NotEquals operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              != dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              != dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Boolean))
    { // Boolean Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Boolean &> (*leftprim).getVal ()
              != dynamic_cast<Boolean &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (String))
    { // String Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<String &> (*leftprim).getVal ()
              != dynamic_cast<String &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for NotEqual operation");
    }

  return ret;
}

// ---------------- LessThan
// Supports Less Than for Integer and Float types
// TODO: Possibly add string comparison?

std::string
LessThan::serialize ()
{
  return std::format ("({} < {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
LessThan::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in LessThan operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              < dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              < dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for LessThan operation");
    }
  return ret;
}

// ---------------- LessThanEqual
// Supports Less Than or Equal for Integer and Float types

std::string
LessThanEqual::serialize ()
{
  return std::format ("({} <= {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
LessThanEqual::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in LessThanEqual operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              <= dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              <= dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error (
          "Unsupported types for LessThanEqual operation");
    }

  return ret;
}

// ---------------- GreaterThan
// Supports Greater Than for Integer and Float types

std::string
GreaterThan::serialize ()
{
  return std::format ("({} > {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
GreaterThan::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in GreaterThan operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              > dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              > dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported types for GreaterThan operation");
    }

  return ret;
}

// ---------------- GreaterThanEqual
// Supports Greater Than or Equal for Integer and Float types

std::string
GreaterThanEqual::serialize ()
{
  return std::format ("({} >= {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
GreaterThanEqual::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftprim = left->evaluate (runtime);
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);

  std::unique_ptr<Primitive> ret;

  if (typeid (*leftprim) != typeid (*rightprim))
    {
      throw std::runtime_error ("Type mismatch in GreaterThanEqual operation");
    }

  if (typeid (*leftprim) == typeid (Integer))
    { // Integer Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Integer &> (*leftprim).getVal ()
              >= dynamic_cast<Integer &> (*rightprim).getVal (),
          -1, -1);
    }
  else if (typeid (*leftprim) == typeid (Float))
    { // Float Case
      ret = std::make_unique<Boolean> (
          dynamic_cast<Float &> (*leftprim).getVal ()
              >= dynamic_cast<Float &> (*rightprim).getVal (),
          -1, -1);
    }
  else
    {
      throw std::runtime_error (
          "Unsupported types for GreaterThanEqual operation");
    }
  return ret;
}

// --------------------- Casting Operations
// --------------------------
//--------------------- FloatToInt
// Requires type to convert is a float or int
std::string
FloatToInt::serialize ()
{
  return std::format ("(int({}))", exp->serialize ());
}

std::unique_ptr<Primitive>
FloatToInt::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> prim = exp->evaluate (runtime);

  if (typeid (*prim) == typeid (Float))
    {

      std::unique_ptr<Primitive> ret = std::make_unique<Integer> (
          static_cast<int> (dynamic_cast<Float &> (*prim).getVal ()), -1, -1);

      return ret;
    }
  else if (typeid (*prim) == typeid (Integer))
    {
      return prim;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for FloatToInt operation");
    }
}

//--------------------- IntToFloat
// Requires type to convert is a int or float
std::string
IntToFloat::serialize ()
{
  return std::format ("(float({}))", exp->serialize ());
}

std::unique_ptr<Primitive>
IntToFloat::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> prim = exp->evaluate (runtime);

  if (typeid (*prim) == typeid (Integer))

    {

      std::unique_ptr<Primitive> ret = std::make_unique<Float> (
          static_cast<float> (dynamic_cast<Integer &> (*prim).getVal ()), -1,
          -1);

      return ret;
    }
  else if (typeid (*prim) == typeid (Float))
    {
      return prim;
    }
  else
    {
      throw std::runtime_error ("Unsupported types for IntToFloatoperation");
    }
}

//--------------------- Statistical Functions --------------------------
//-------------- Max
// Iterates in row-major order, finds the max.

std::string
Max::serialize ()
{
  return std::format ("(max({}, {}))", left->serialize (),
                      right->serialize ());
}

std::unique_ptr<Primitive>
Max::evaluate (std::shared_ptr<Runtime> runtime)
{
  // All four stats functions perform a similar traversal.
  // Consider factoring out some of the grossness to a helper method.

  std::unique_ptr<Primitive> leftAddress = (left->evaluate (runtime));
  std::unique_ptr<Primitive> rightAddress = (right->evaluate (runtime));

  if (typeid (*leftAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid left address");
    }

  if (typeid (*rightAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid right address");
    }

  int leftRow = dynamic_cast<CellAddress &> (*leftAddress).getRow ();
  int leftCol = dynamic_cast<CellAddress &> (*leftAddress).getCol ();
  int rightRow = dynamic_cast<CellAddress &> (*rightAddress).getRow ();
  int rightCol = dynamic_cast<CellAddress &> (*rightAddress).getCol ();

  if (leftRow > rightRow || leftCol > rightCol)
    {
      throw std::runtime_error (
          "Cells must be ordered (topLeft, bottomRight)");
    }

  double max = -INFINITY;

  for (int i = leftRow; i <= rightRow; i++)
    {
      for (int j = leftCol; j <= rightCol; j++)
        {
          CellAddress address (i, j, -1, -1); // -1 is a placeholder

          std::unique_ptr<Primitive> cellprim
              = runtime->getCell (&address, runtime);
          if (cellprim == nullptr || typeid (*cellprim) == typeid (Boolean)
              || typeid (*cellprim) == typeid (String)
              || typeid (*cellprim) == typeid (CellAddress))
            {
              continue; // Skip empty cells, design choice
            }
          else if (typeid (*cellprim) == typeid (Integer))
            {
              int val = dynamic_cast<Integer &> (*cellprim).getVal ();
              if (val > max)
                {
                  max = val;
                }
            }
          else if (typeid (*cellprim) == typeid (Float))
            {
              double val = dynamic_cast<Float &> (*cellprim).getVal ();
              if (val > max)
                {
                  max = val;
                }
            }
          else
            {
              throw std::runtime_error ("Unsupported type in Sum operation "
                                        "(Supports Integers and Floats)");
            }
        }
    }

  return std::make_unique<Float> (max, -1, -1);
}

//-------------- Min
// Iterates in row-major order, finds the min.

std::string
Min::serialize ()
{
  return std::format ("(min({}, {}))", left->serialize (),
                      right->serialize ());
}

std::unique_ptr<Primitive>
Min::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftAddress = (left->evaluate (runtime));
  std::unique_ptr<Primitive> rightAddress = (right->evaluate (runtime));

  if (typeid (*leftAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid left address");
    }

  if (typeid (*rightAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid right address");
    }

  int leftRow = dynamic_cast<CellAddress &> (*leftAddress).getRow ();
  int leftCol = dynamic_cast<CellAddress &> (*leftAddress).getCol ();
  int rightRow = dynamic_cast<CellAddress &> (*rightAddress).getRow ();
  int rightCol = dynamic_cast<CellAddress &> (*rightAddress).getCol ();

  if (leftRow > rightRow || leftCol > rightCol)
    {
      throw std::runtime_error (
          "Cells must be ordered (topLeft, bottomRight)");
    }

  double min = INFINITY;

  for (int i = leftRow; i <= rightRow; i++)
    {
      for (int j = leftCol; j <= rightCol; j++)
        {
          CellAddress address (i, j, -1, -1); // -1 is a placeholder

          std::unique_ptr<Primitive> cellprim
              = runtime->getCell (&address, runtime);
          if (cellprim == nullptr || typeid (*cellprim) == typeid (Boolean)
              || typeid (*cellprim) == typeid (String)
              || typeid (*cellprim) == typeid (CellAddress)) // DANGER
            {
              continue; // Skip empty cells, design choice
            }
          else if (typeid (*cellprim) == typeid (Integer))
            {
              int val = dynamic_cast<Integer &> (*cellprim).getVal ();
              if (val < min)
                {
                  min = val;
                }
            }
          else if (typeid (*cellprim) == typeid (Float))
            {
              double val = dynamic_cast<Float &> (*cellprim).getVal ();
              if (val < min)
                {
                  min = val;
                }
            }
          else
            {
              throw std::runtime_error ("Unsupported type in Sum operation "
                                        "(Supports Integers and Floats)");
            }
        }
    }

  return std::make_unique<Float> (min, -1, -1);
}

//-------------- Mean
// Iterates in row-major order, adds

std::string
Mean::serialize ()
{
  return std::format ("mean({}, {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
Mean::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> leftAddress = (left->evaluate (runtime));
  std::unique_ptr<Primitive> rightAddress = (right->evaluate (runtime));

  if (typeid (*leftAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid left address");
    }

  if (typeid (*rightAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid right address");
    }

  int leftRow = dynamic_cast<CellAddress &> (*leftAddress).getRow ();
  int leftCol = dynamic_cast<CellAddress &> (*leftAddress).getCol ();
  int rightRow = dynamic_cast<CellAddress &> (*rightAddress).getRow ();
  int rightCol = dynamic_cast<CellAddress &> (*rightAddress).getCol ();

  if (leftRow > rightRow || leftCol > rightCol)
    {
      throw std::runtime_error (
          "Cells must be ordered (topLeft, bottomRight)");
    }

  int count = 0;
  double sum = 0;

  for (int i = leftRow; i <= rightRow; i++)
    {
      for (int j = leftCol; j <= rightCol; j++)
        {
          CellAddress address (i, j, -1, -1); // -1 is a placeholder

          std::unique_ptr<Primitive> cellprim
              = runtime->getCell (&address, runtime);
          if (cellprim == nullptr || typeid (*cellprim) == typeid (Boolean)
              || typeid (*cellprim) == typeid (String)
              || typeid (*cellprim) == typeid (CellAddress)) // DANGER
            {
              continue; // Skip empty cells, design choice
            }
          else if (typeid (*cellprim) == typeid (Integer))
            {
              count += 1;
              sum += dynamic_cast<Integer &> (*cellprim).getVal ();
            }
          else if (typeid (*cellprim) == typeid (Float))
            {
              count += 1;
              sum += dynamic_cast<Float &> (*cellprim).getVal ();
            }
          else
            {
              throw std::runtime_error ("Unsupported type in Sum operation "
                                        "(Supports Integers and Floats)");
            }
        }
    }

  if (count == 0)
    {
      return std::make_unique<Float> (0, -1, -1); // Avoid division by zero
    }

  return std::make_unique<Float> (sum / count, -1, -1);
}

//-------------- Sum
std::string
Sum::serialize ()
{
  return std::format ("sum({}, {})", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
Sum::evaluate (std::shared_ptr<Runtime> runtime)
{
  double sum = 0;

  std::unique_ptr<Primitive> leftAddress = (left->evaluate (runtime));
  std::unique_ptr<Primitive> rightAddress = (right->evaluate (runtime));

  if (typeid (*leftAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid left address");
    }

  if (typeid (*rightAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid right address");
    }

  int leftRow = dynamic_cast<CellAddress &> (*leftAddress).getRow ();
  int leftCol = dynamic_cast<CellAddress &> (*leftAddress).getCol ();
  int rightRow = dynamic_cast<CellAddress &> (*rightAddress).getRow ();
  int rightCol = dynamic_cast<CellAddress &> (*rightAddress).getCol ();

  if (leftRow > rightRow || leftCol > rightCol)
    {
      throw std::runtime_error (
          "Cells must be ordered (topLeft, bottomRight)");
    }

  for (int i = leftRow; i <= rightRow; i++)
    {
      for (int j = leftCol; j <= rightCol; j++)
        {
          CellAddress address (i, j, -1, -1); // -1 is a placeholder

          std::unique_ptr<Primitive> cellprim
              = runtime->getCell (&address, runtime);
          if (cellprim == nullptr || typeid (*cellprim) == typeid (Boolean)
              || typeid (*cellprim) == typeid (String)
              || typeid (*cellprim) == typeid (CellAddress)) // DANGER
            {
              continue; // Skip empty cells, design choice
            }
          else if (typeid (*cellprim) == typeid (Integer))
            {
              sum += dynamic_cast<Integer &> (*cellprim).getVal ();
            }
          else if (typeid (*cellprim) == typeid (Float))
            {
              sum += dynamic_cast<Float &> (*cellprim).getVal ();
            }
          else
            {
              throw std::runtime_error ("Unsupported type in Sum operation "
                                        "(Supports Integers and Floats)");
            }
        }
    }
  return std::make_unique<Float> (sum, -1, -1);
}

// --------------------- Blocks, Variables, and Assignments
// --------------- Block
std::string
Block::serialize ()
{
  std::string ret = "Block: {";
  for (std::unique_ptr<Expression> &statement : statements)
    {
      ret += statement->serialize ();
      ret += "; ";
    }
  return ret;
}

std::unique_ptr<Primitive>
Block::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> ret;
  for (std::unique_ptr<Expression> &statement : statements)
    {
      // Runs each statement, then returns whatever the last one evaluates to
      ret = statement->evaluate (runtime);
    }
  return ret;
}

// --------------- Variable
std::string
Variable::serialize ()
{
  return std::format ("Variable:{}", name);
}

std::unique_ptr<Primitive>
Variable::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> ret = runtime->getVariable (name);
  if (ret == nullptr)
    {
      throw std::runtime_error ("Undefined variable found");
    }
  return ret;
}

std::string
Variable::getName ()
{
  return name;
}

// --------------- Assignment
std::string
Assignment::serialize ()
{
  return std::format ("{} = {}", left->serialize (), right->serialize ());
}

std::unique_ptr<Primitive>
Assignment::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> rightprim = right->evaluate (runtime);
  // if (typeid (*left) != typeid (Variable))
  //   {
  //     throw std::runtime_error (
  //         "Left hand side of assignment must be a variable");
  //   }

  runtime->setVariable (dynamic_cast<Variable &> (*left).getName (),
                        std::move (rightprim));

  rightprim = right->evaluate (runtime);

  return rightprim;
}

// --------------- IfExpr
std::string
IfExpr::serialize ()
{
  return std::format ("IF <{}>: <{}> ELSE <{}>", condition->serialize (),
                      ifTrue->serialize (), ifFalse->serialize ());
}

std::unique_ptr<Primitive>
IfExpr::evaluate (std::shared_ptr<Runtime> runtime)
{
  std::unique_ptr<Primitive> conditionprim = condition->evaluate (runtime);

  if (typeid (*conditionprim) != typeid (Boolean))
    {
      throw std::runtime_error ("Condition must evaluate to a boolean");
    }

  if (dynamic_cast<Boolean &> (*conditionprim).getVal ())
    {
      return ifTrue->evaluate (runtime);
    }
  else
    {
      return ifFalse->evaluate (runtime);
    }
}

// -------------- ForExpr
std::string
ForExpr::serialize ()
{
  return std::format ("FOR <{}> IN {}..{}: <{}>", variable->serialize (),
                      left->serialize (), right->serialize (),
                      block->serialize ());
}

std::unique_ptr<Primitive>
ForExpr::evaluate (std::shared_ptr<Runtime> runtime)
{
  if (typeid (*variable) != typeid (Variable))
    {
      throw std::runtime_error ("Invalid variable in For loop");
    }

  std::unique_ptr<Primitive> leftAddress = (left->evaluate (runtime));
  std::unique_ptr<Primitive> rightAddress = (right->evaluate (runtime));

  if (typeid (*leftAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid left address");
    }

  if (typeid (*rightAddress) != typeid (CellAddress))
    {
      throw std::runtime_error ("Invalid right address");
    }

  int leftRow = dynamic_cast<CellAddress &> (*leftAddress).getRow ();
  int leftCol = dynamic_cast<CellAddress &> (*leftAddress).getCol ();
  int rightRow = dynamic_cast<CellAddress &> (*rightAddress).getRow ();
  int rightCol = dynamic_cast<CellAddress &> (*rightAddress).getCol ();

  if (leftRow > rightRow || leftCol > rightCol)
    {
      throw std::runtime_error (
          "Cells must be ordered (topLeft, bottomRight)");
    }

  std::unique_ptr<Primitive> ret;

  for (int i = leftRow; i <= rightRow; i++)
    {
      for (int j = leftCol; j <= rightCol; j++)
        {
          std::unique_ptr<CellAddress> cellAddress
              = std::make_unique<CellAddress> (i, j, -1, -1);
          std::unique_ptr<Primitive> cellprim
              = runtime->getCell (cellAddress.get (), runtime);
          runtime->setVariable (
              dynamic_cast<Variable &> (*variable).getName (),
              std::move (cellprim));

          ret = block->evaluate (runtime);
        }
    }

  return ret;
}
// End
