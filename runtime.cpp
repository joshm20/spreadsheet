#include "runtime.h"
#include <memory>

Runtime::Runtime (std::shared_ptr<Grid> grid) : grid (grid) {};

std::unique_ptr<Primitive>
Runtime::getCell (CellAddress *address, std::shared_ptr<Runtime> runtime)
{
  return grid->getValue (address, runtime);
}

void
Runtime::setVariable (std::string name, std::unique_ptr<Primitive> value)
{
  variables[name] = std::move (value);
}

std::unique_ptr<Primitive>
Runtime::getVariable (std::string name)
{
  // Weirdness follows
  // Make a copy of the variable based on the type of variables[name]

  if (variables[name] == nullptr)
    {
      return std::make_unique<Integer> (0, -1, -1);
    }

  if (typeid (*(variables[name])) == typeid (Integer))
    {
      return std::make_unique<Integer> (
          dynamic_cast<Integer &> (*variables[name]).getVal (), -1, -1);
    }
  else if (typeid (*(variables[name])) == typeid (Float))
    {
      return std::make_unique<Float> (
          dynamic_cast<Float &> (*variables[name]).getVal (), -1, -1);
    }
  else if (typeid (*(variables[name])) == typeid (Boolean))
    {
      return std::make_unique<Boolean> (
          dynamic_cast<Boolean &> (*variables[name]).getVal (), -1, -1);
    }
  else if (typeid (*(variables[name])) == typeid (String))
    {
      return std::make_unique<String> (
          dynamic_cast<String &> (*variables[name]).getVal (), -1, -1);
    }
  else if (typeid (*(variables[name])) == typeid (CellAddress))
    {
      return std::make_unique<CellAddress> (
          dynamic_cast<CellAddress &> (*variables[name]).getRow (),
          dynamic_cast<CellAddress &> (*variables[name]).getCol (), -1, -1);
    }
  else
    {
      throw std::runtime_error ("Unsupported type in getVariable");
    }
}

Runtime::~Runtime () {}