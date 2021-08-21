#ifndef CURSESWRAPPER_HPP
#define CURSESWRAPPER_HPP

#include <curses.h>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <string>

enum class Color
{
	none,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan
};

namespace CursesWrapper
{
	
void init_color()
{
	if ( has_colors() && COLOR_PAIRS >= 7 )
	{
		init_pair(1,  COLOR_RED,     COLOR_BLACK);
		init_pair(2,  COLOR_GREEN,   COLOR_BLACK);
		init_pair(3,  COLOR_YELLOW,  COLOR_BLACK);
		init_pair(4,  COLOR_BLUE,    COLOR_BLACK);
		init_pair(5,  COLOR_MAGENTA, COLOR_BLACK);
		init_pair(6,  COLOR_CYAN,    COLOR_BLACK);
	}
}

void init_curses(WINDOW *mainwin)
{
	setlocale(LC_ALL, "");
    initscr();
	keypad(mainwin, true);
	cbreak();
	noecho();
	nodelay(mainwin, true);
	//signal(SIGWINCH, handle_sigwinch);
	CursesWrapper::init_color();
}

std::string& handle_input(WINDOW *win, int ch)
{
	static std::string str;
	static auto str_it = str.end();
	static int pos{0};
	if( ch != ERR )
	{	
		ch = getch();
		switch(ch)
		{
		case 9: //tab
			for(int i=0;i<4;++i)
			{
				winsch(win,' ');
				wmove(win,getcury(win),getcurx(win)+1);
				str_it = str.insert(str_it,' ');
				++str_it;
			}
			break;
		case 127: //backspace
			if(getcurx(win) > 0)
			{
				wmove(win,getcury(win),getcurx(win)-1);
				wdelch(win);
				if(pos > 0)
					++pos;
				if(str_it != str.begin())
					str_it = str.erase(--str_it);
			}
			break;
		case 10: //enter
			printw("\n you entered: %s\n", str.c_str());
			str.clear();
			pos = 0;
			str_it = str.end();
			break;
		case KEY_DC: //delete
			wdelch(win);
			if(pos > 0)
				--pos;
			if(str_it != str.end())
					str_it = str.erase(str_it);
			break;
		case KEY_UP:
			break;
		case KEY_DOWN:
			break;
		case KEY_LEFT:
			if(getcurx(win) > 0)
			{
				wmove(win,getcury(win),getcurx(win)-1);
				++pos;
				if(str_it != str.begin())
					--str_it;
			}
			break;
		case KEY_RIGHT:
			if(pos > 0)
			{
				wmove(win,getcury(win),getcurx(win)+1);
				--pos;
				if(str_it != str.end())
					++str_it;
			}
			break;
		default:
			if(ch >= 32 && ch <=126)
			{
				winsch(win, ch);
				wmove(win,getcury(win),getcurx(win)+1);
				str_it = str.insert(str_it,ch);
				++str_it;
			}
			else
				printw(" %s ",keyname(ch));
			break;
		}
	}
	return str;
}

}
#endif