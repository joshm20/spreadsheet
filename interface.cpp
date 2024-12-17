#include "interface.h"
#include "lexer.h"
#include "parser.h"
#include "runtime.h"
#include <memory>
#include <ncurses.h>
#include <string>

Interface::Interface ()
{
  int height = LINES;
  int width = COLS;
  editor_dim = { 0, 0, width, 7 };
  output_dim = { 0, editor_dim.height, width / 2, 3 };
  error_dim = { output_dim.width, editor_dim.height, width - output_dim.width,
                output_dim.height };
  grid_dim = { 0, editor_dim.height + output_dim.height, width,
               height - editor_dim.height - output_dim.height };

  cur_row = 0; // Current row
  cur_col = 0; // Current column
  cur_x = 0;   // Current x position
  cur_y = 0;   // Current y position
}

Interface::~Interface () { this->deleteWindows (); }

void
Interface::deleteWindows ()
{
  if (!grid_win)
    {
      delwin (grid_win);
      grid_win = nullptr;
    }
  if (!editor_win)
    {
      delwin (editor_win);
      editor_win = nullptr;
    }
  if (!output_win)
    {
      delwin (output_win);
      output_win = nullptr;
    }
  if (!error_win)
    {
      delwin (error_win);
      error_win = nullptr;
    }
}

void
Interface::makeWindows ()
{
  this->deleteWindows ();

  editor_win = newwin (editor_dim.height, editor_dim.width, editor_dim.y,
                       editor_dim.x);
  output_win = newwin (output_dim.height, output_dim.width, output_dim.y,
                       output_dim.x);
  error_win
      = newwin (error_dim.height, error_dim.width, error_dim.y, error_dim.x);
  grid_win = newwin (grid_dim.height, grid_dim.width, grid_dim.y, grid_dim.x);
}

void
Interface::drawBorders ()
{
  this->makeWindows ();

  box (editor_win, 0, 0);
  box (output_win, 0, 0);
  box (grid_win, 0, 0);
  box (error_win, 0, 0);

  refresh ();
  wrefresh (editor_win);
  wrefresh (output_win);
  wrefresh (error_win);
  wrefresh (grid_win);
}

void
Interface::shrinkWindows ()
{
  this->deleteWindows ();

  grid_dim.height -= 2;
  grid_dim.width -= 2;
  grid_dim.y += 1;
  grid_dim.x += 1;

  editor_dim.height -= 2;
  editor_dim.width -= 2;
  editor_dim.y += 1;
  editor_dim.x += 1;

  output_dim.height -= 2;
  output_dim.width -= 2;
  output_dim.y += 1;
  output_dim.x += 1;

  error_dim.height -= 2;
  error_dim.width -= 2;
  error_dim.y += 1;
  error_dim.x += 1;

  this->makeWindows ();
}

void
Interface::drawGridLines ()
{
  // Draw a line every other row (improve logic?)
  for (int i = 1; i < grid_dim.height; i += 2)
    {
      mvwhline (grid_win, i, grid_dim.x - 1, 0, grid_dim.width);
    }
  // Draw vertical lines, (improve logic?)
  for (int i = 15; i < grid_dim.width; i += 16)
    {
      mvwvline (grid_win, grid_dim.y - 11, i, 0, grid_dim.height);
    }

  wrefresh (editor_win);
  wrefresh (output_win);
  wrefresh (grid_win);
  wmove (grid_win, 0, 0);
}

// ---------------- Grid Loop ----------------
void
Interface::gridLoop ()
{
  // Declared in grid.h, I don't think C allows variables to be taken when
  // making a a 2D array, which is what would be made if I called Grid() with
  // row and column parameters. These are also arbitrary to make it look good
  // on my screen size. Possibly improve in the future?
  int grid_row_len = 20;
  int grid_col_len = 13;
  int c;

  std::shared_ptr<Grid> grid = std::make_shared<Grid> ();
  std::shared_ptr<Runtime> runtime = std::make_shared<Runtime> (grid);
  std::unique_ptr<CellAddress> cur_celladdr
      = std::make_unique<CellAddress> (cur_row, cur_col, -1, -1);

  while (true)
    {
      // Print current cell in output
      cur_celladdr = std::make_unique<CellAddress> (cur_row, cur_col, -1, -1);
      runtime = nullptr;
      runtime = std::make_shared<Runtime> (grid);

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

      cur_x = getcurx (grid_win);
      cur_y = getcury (grid_win);

      this->drawGridPrimitives (grid, runtime);

      wmove (grid_win, cur_y, cur_x);

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
            std::string source = this->editorLoop (current_source);
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
}

// ---------------- Editor Loop ----------------
std::string
Interface::editorLoop (std::string source)
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
          // NOT IMPLEMENTED
        case KEY_UP:
          break;
        //   wmove (editor_win, getcury (editor_win) - 1, getcurx
        //   (editor_win)); break;
        case KEY_DOWN:
          break;
        //   wmove (editor_win, getcury (editor_win) + 1, getcurx
        //   (editor_win)); break;
        case KEY_LEFT:
          break;
        //   wmove (editor_win, getcury (editor_win), getcurx (editor_win) -
        //   1); break;
        case KEY_RIGHT:
          break;
        //   wmove (editor_win, getcury (editor_win), getcurx (editor_win) +
        //   1); break;
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

void
Interface::drawGridPrimitives (std::shared_ptr<Grid> grid,
                               std::shared_ptr<Runtime> runtime)
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
  CellAddress curaddr (cur_row, cur_col, -1, -1);
  std::unique_ptr<Primitive> cur_value = grid->getValue (&curaddr, runtime);
  std::string cur_str = cur_value->serialize ().substr (0, 15);
  cur_str += std::string (15 - cur_str.length (), ' ');
  mvwprintw (grid_win, cur_y, cur_x, "%s", cur_str.c_str ());
  wattr_off (grid_win, A_REVERSE, NULL);
}