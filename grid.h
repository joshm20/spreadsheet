#ifndef grid_H
#define grid_H

#include "cell.h"
#include "expression.h"
#include "forward_declarations.h"
#include <iostream>
#include <memory>
#include <string>

/* The Grid class holds a 2D array of pointers to Cells. The rows and columns
 * are specifically hardcoded to fit my screen on a linux system (NOT
 * PORTABLE).
 *
 * @author Josh Makela
 * @date 12-17-2024
 */
class Grid
{
private:
  // Data structure to store multiple cells
  static constexpr int rows = 20;
  static constexpr int cols = 13;
  std::shared_ptr<Cell> cells[rows][cols];

public:
  Grid ();
  int
  getRows ()
  {
    return rows;
  }
  int
  getCols ()
  {
    return cols;
  }
  std::shared_ptr<Cell> getCell (int row, int col);
  void setCell (int row, int col, std::string src,
                std::unique_ptr<Expression> exp,
                std::shared_ptr<Runtime> runtime, std::string error);

  // Returns null if cell is uninitialized, Primitive otherwise
  std::unique_ptr<Primitive> getValue (CellAddress *address,
                                       std::shared_ptr<Runtime> runtime);

  // Function to print the grid for debugging
  void printGrid (std::shared_ptr<Runtime> runtime);

  void updateGrid (std::shared_ptr<Runtime> runtime);

  // Function to get a reference to the cells array
  std::shared_ptr<Cell> **getCells ();

  ~Grid ();
};

#endif
