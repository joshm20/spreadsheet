#include "runtime.h"
#include <memory>
#include <ncurses.h>
#include <string>

typedef struct Dimension_t
{
  int x;
  int y;
  int width;
  int height;
} Dimension_t;

class Interface
{
private:
  Dimension_t grid_dim;
  Dimension_t editor_dim;
  Dimension_t output_dim;
  Dimension_t error_dim;
  WINDOW *grid_win;
  WINDOW *editor_win;
  WINDOW *output_win;
  WINDOW *error_win;
  int cur_row;
  int cur_col;
  int cur_x;
  int cur_y;

  std::string editorLoop (std::string source);
  void drawGridPrimitives (std::shared_ptr<Grid> grid,
                           std::shared_ptr<Runtime> runtime);
  void makeWindows ();
  void deleteWindows ();

public:
  Interface ();
  ~Interface ();

  void drawBorders ();
  void shrinkWindows ();
  void drawGridLines ();
  void gridLoop ();
};