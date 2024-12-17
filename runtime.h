#ifndef runtime_H
#define runtime_H

#include "forward_declarations.h"
#include "grid.h"
#include <memory>
#include <unordered_map>

// Runtime contains a pointer to a Grid object, and will contain more
// information in the future.
class Runtime
{
private:
  // This could probably be a shared_ptr since you want just one grid passed
  // around.
  std::shared_ptr<Grid> grid;
  std::unordered_map<std::string, std::unique_ptr<Primitive> > variables;

public:
  Runtime (std::shared_ptr<Grid> grid);
  std::unique_ptr<Primitive> getCell (CellAddress *address,
                                      std::shared_ptr<Runtime> runtime);

  void setVariable (std::string name, std::unique_ptr<Primitive> value);
  std::unique_ptr<Primitive> getVariable (std::string name);

  ~Runtime ();
};

#endif
