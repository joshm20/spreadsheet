#include "interface.h"
#include <ncurses.h>

int
main ()
{

  initscr ();
  cbreak (); // Explicitly disable IO buffering
  noecho (); // Don't automatically print any keypresses
  // curs_set (0); // Hide the cursor
  keypad (stdscr, TRUE);
  Interface interface;

  interface.drawBorders ();
  interface.shrinkWindows ();
  interface.drawGridLines ();
  interface.gridLoop ();

  endwin ();

  return EXIT_SUCCESS;
}