#ifndef CURSESWRAPPER_HPP
#define CURSESWRAPPER_HPP

#include <curses.h>
#include <string>

enum class Color
{
	white,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan
};

namespace CursesWrapper
{
	
void init_color();

void start_curses();

void configureWindows(WINDOW *chatwin, WINDOW *inputwin);

void deleteWindows(WINDOW *chatwin, WINDOW *inputwin);

bool handle_input(WINDOW *win, std::string& outstr, int ch);

}
#endif