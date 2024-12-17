require 'curses'

include Curses

init_screen
noecho
curs_set(0)

input_panel = Window.new(1, 30, 0, 0)
input_panel.keypad(true)

loop do
  input_panel.setpos(0, 0)
  c = input_panel.getch
  if c == 'q'
    break
  else
    input_panel.clear
    input_panel.addstr(c.ord.to_s)
  end
end

input_panel.close
close_screen