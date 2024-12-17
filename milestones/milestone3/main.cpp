#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <ncurses.h>
#include <string>
#include <vector>

// A class is a reasonable organizational structure for all this code, even
// though you only expect to make a single instance of the interface. It
// provides a conventional place for initialization, isolates the globals into
// a narrower scope, and gives you the opportunity to break operations into
// methods with meaningful names. Top-level methods are okay, but you need to
// either push shared data into globals or pass around a bunch of parameters.
// Classes tidy everything and make for clean sharing.

std::string editor_loop (WINDOW *editor_win, std::string source);
void drawGrid (WINDOW *grid_win, std::shared_ptr<Grid> grid,
               std::shared_ptr<Runtime> runtime, int cur_rpw, int cur_col,
               int save_x, int save_y);

int
main ()
{
  initscr ();
  cbreak (); // Explicitly disable IO buffering
  noecho (); // Don't automatically print any keypresses
  // curs_set (0); // Hide the cursor
  keypad (stdscr, TRUE);
  // Thorough parameterization.
  int height = LINES;
  int width = COLS;

  int editor_start_x = 0;
  int editor_start_y = 0;
  int editor_width = width;
  int editor_height = 6;

  int output_start_x = 0;
  int output_start_y = editor_height;
  int output_width = width / 2;
  int output_height = 4;

  int error_start_x = output_width;
  int error_start_y = editor_height;
  int error_width = width - output_width;
  int error_height = output_height;

  int grid_start_x = 0;
  int grid_start_y = editor_height + output_height;
  int grid_width = width;
  int grid_height = height - editor_height - output_height;

  WINDOW *grid_win
      = newwin (grid_height, grid_width, grid_start_y, grid_start_x);
  WINDOW *editor_win
      = newwin (editor_height, editor_width, editor_start_y, editor_start_x);
  WINDOW *output_win
      = newwin (output_height, output_width, output_start_y, output_start_x);
  WINDOW *error_win
      = newwin (error_height, error_width, error_start_y, error_start_x);

  // Draw borders
  box (editor_win, 0, 0);
  box (output_win, 0, 0);
  box (grid_win, 0, 0);
  box (error_win, 0, 0);

  // Shouldn't need to call refresh if you've partitioned the screen into
  // windows.
  refresh ();
  wrefresh (grid_win);
  wrefresh (editor_win);
  wrefresh (output_win);
  wrefresh (error_win);
  // Free windows
  delwin (grid_win);
  delwin (editor_win);
  delwin (output_win);
  delwin (error_win);

  // Shrink into borders
  // Clever dilation to handle the border vs. content dilemma.
  grid_height -= 2;
  grid_width -= 2;
  grid_start_y += 1;
  grid_start_x += 1;

  editor_height -= 2;
  editor_width -= 2;
  editor_start_y += 1;
  editor_start_x += 1;

  output_height -= 2;
  output_width -= 2;
  output_start_y += 1;
  output_start_x += 1;

  error_height -= 2;
  error_width -= 2;
  error_start_y += 1;
  error_start_x += 1;

  // Establish drawable windows
  grid_win = newwin (grid_height, grid_width, grid_start_y, grid_start_x);
  editor_win
      = newwin (editor_height, editor_width, editor_start_y, editor_start_x);
  output_win
      = newwin (output_height, output_width, output_start_y, output_start_x);
  error_win = newwin (error_height, error_width, error_start_y, error_start_x);

  // Draw a line every other row (improve logic?)
  for (int i = 1; i < grid_height; i += 2)
    {
      mvwhline (grid_win, i, grid_start_x - 1, 0, grid_width);
    }
  // Draw vertical lines, (improve logic?)
  for (int i = 15; i < grid_width; i += 16)
    {
      mvwvline (grid_win, grid_start_y - 11, i, 0, grid_height);
    }

  wrefresh (editor_win);
  wrefresh (output_win);
  wrefresh (grid_win);
  wmove (grid_win, 0, 0);

  int c;
  int cur_row = 0; // Current row
  int cur_col = 0; // Current column
  // Declared in grid.h, I don't think C allows variables to be taken when
  // making a a 2D array, which is what would be made if I called Grid() with
  // row and column parameters. These are also arbitrary to make it look good
  // on my screen size. Possibly improve in the future?
  int grid_row_len = 20;
  int grid_col_len = 13;

  std::shared_ptr<Grid> grid = std::make_shared<Grid> ();
  std::shared_ptr<Runtime> runtime = std::make_shared<Runtime> (grid);
  std::unique_ptr<CellAddress> cur_celladdr
      = std::make_unique<CellAddress> (cur_row, cur_col, -1, -1);

  // Grid loop
  while (true)
    {
      // Print current cell in output
      cur_celladdr = std::make_unique<CellAddress> (cur_row, cur_col, -1, -1);

      werase (editor_win);
      werase (output_win);
      werase (error_win);

      std::shared_ptr<Cell> cell = grid->getCell (cur_row, cur_col);
      std::string output = cell->getPrimitive (runtime)->serialize ();
      std::string current_source = cell->getString ();
      std::string error = cell->getError ();

      waddstr (editor_win, current_source.c_str ());
      waddstr (output_win, output.c_str ());
      waddstr (error_win, error.c_str ());

      int save_x = getcurx (grid_win);
      int save_y = getcury (grid_win);

      drawGrid (grid_win, grid, runtime, cur_row, cur_col, save_x, save_y);

      wmove (grid_win, save_y, save_x);

      wrefresh (editor_win);
      wrefresh (output_win);
      wrefresh (error_win);
      wrefresh (grid_win);

      c = getch ();
      switch (c)
        {
        case KEY_UP:
          wmove (grid_win, getcury (grid_win) - 2, getcurx (grid_win));
          if (cur_row > 0)
            cur_row--;
          break;
        case KEY_DOWN:
          wmove (grid_win, getcury (grid_win) + 2, getcurx (grid_win));
          if (cur_row < grid_row_len - 1)
            cur_row++;
          break;
        case KEY_LEFT:
          wmove (grid_win, getcury (grid_win), getcurx (grid_win) - 16);
          if (cur_col > 0)
            cur_col--;
          break;
        case KEY_RIGHT:
          wmove (grid_win, getcury (grid_win), getcurx (grid_win) + 16);
          if (cur_col < grid_col_len - 1)
            cur_col++;
          break;
        case KEY_ENTER:
          break;
        case KEY_END:
          { // Block scope to supress warnings
            // Enter cell edit mode
            std::string source = editor_loop (editor_win, current_source);
            try
              {
                Lexer lexer = Lexer (source);
                Parser parser = Parser (*(lexer.lex ()));
                grid->setCell (cur_row, cur_col, source, parser.parse (),
                               runtime, "");
              }
            catch (std::exception &e)
              {
                grid->setCell (cur_row, cur_col, source,
                               std::make_unique<String> ("NULL", 0, 0),
                               runtime, e.what ());
              }
            grid->updateGrid (runtime);
          }
          break;
        default:
          break;
        }

      wrefresh (output_win);
      wrefresh (editor_win);
      wrefresh (grid_win);
    }

  endwin ();

  return EXIT_SUCCESS;
}

// Handles the editing of the cell. Source is the current source string of the
// cell. Returns the new source string.
std::string
editor_loop (WINDOW *editor_win, std::string source)
{
  wrefresh (editor_win);
  std::string new_source = source;
  int c;
  bool exit = false;
  while (!exit)
    {
      c = getch ();
      switch (c)
        {
        case KEY_UP:
          wmove (editor_win, getcury (editor_win) - 1, getcurx (editor_win));
          break;
        case KEY_DOWN:
          wmove (editor_win, getcury (editor_win) + 1, getcurx (editor_win));
          break;
        case KEY_LEFT:
          wmove (editor_win, getcury (editor_win), getcurx (editor_win) - 1);
          break;
        case KEY_RIGHT:
          wmove (editor_win, getcury (editor_win), getcurx (editor_win) + 1);
          break;
        case 10: // Enter key
                 // Check that adding a newline does not go out of the window
          if (getcury (editor_win) < getmaxy (editor_win) - 1)
            {
              new_source += '\n';
              waddch (editor_win, c);
            }
          break;
        case KEY_DC: // May choose to not support this later
        case KEY_BACKSPACE:
          // If the last character in source is a new line, remove it, then
          // move up a line and go to the end of the line
          if (!new_source.empty () && new_source.back () == '\n')
            {
              new_source.pop_back ();
              wmove (editor_win, getcury (editor_win) - 1,
                     getmaxx (editor_win) - 1);
              while (winch (editor_win) == ' ' && getcurx (editor_win) > 0)
                {
                  wmove (editor_win, getcury (editor_win),
                         getcurx (editor_win) - 1);
                }
              // Overshoots, so recorrect
              wmove (editor_win, getcury (editor_win),
                     getcurx (editor_win) + 1);
            }
          // Otherwise, just remove the last character
          else
            {
              if (!new_source.empty ())
                new_source.pop_back ();
              wmove (editor_win, getcury (editor_win),
                     getcurx (editor_win) - 1);
              wdelch (editor_win);
            }

          break;
        case KEY_END:
          exit = true;
          break;
        default:
          new_source += c;
          waddch (editor_win, c);
          break;
        }

      refresh ();
      wrefresh (editor_win);
    }

  return new_source;
}

// Draw all primitive values to the grid, does not refresh the window
void
drawGrid (WINDOW *grid_win, std::shared_ptr<Grid> grid,
          std::shared_ptr<Runtime> runtime, int cell_x, int cell_y, int cur_x,
          int cur_y)
{
  int i = 0;
  int y = 0;
  while (i < grid->getRows ())
    {
      int j = 0;
      int x = 0;
      while (j < grid->getCols ())
        {
          CellAddress address (i, j, -1, -1);
          std::unique_ptr<Primitive> value
              = grid->getValue (&address, runtime);

          if (value)
            {
              std::string str = value->serialize ().substr (
                  0, 15); // Limit to 15 characters
                          // If primitive is empty string, just draw spaces
              str += std::string (15 - str.length (),
                                  ' '); // Pad with spaces
              mvwprintw (grid_win, y, x, "%s", str.c_str ());
            }
          x += 16; // Move to the next column
          ++j;
        }
      y += 2; // Move to the next row
      ++i;
    }
  // Move cursor to cur_x and cur_y, then print the cell primitive in reverse
  // video attribute.
  wattr_on (grid_win, A_REVERSE, NULL);
  CellAddress curaddr (cell_x, cell_y, -1, -1);
  std::unique_ptr<Primitive> cur_value = grid->getValue (&curaddr, runtime);
  std::string cur_str = cur_value->serialize ().substr (0, 15);
  cur_str += std::string (15 - cur_str.length (), ' ');
  mvwprintw (grid_win, cur_y, cur_x, "%s", cur_str.c_str ());
  wattr_off (grid_win, A_REVERSE, NULL);
}
