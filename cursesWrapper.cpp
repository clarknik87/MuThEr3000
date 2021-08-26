#include "cursesWrapper.hpp"
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <curses.h>
#include <iostream>

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
	color_set(2, nullptr);
}

void start_curses()
{
	setlocale(LC_ALL, "");
    initscr();
	cbreak();
	noecho();
	//signal(SIGWINCH, handle_sigwinch);
	CursesWrapper::init_color();
}

void draw_borders(WINDOW *chatborder, WINDOW *inputborder)
{	
	box(stdscr, 0, 0);
	box(chatborder, 0, 0);
	box(inputborder, 0, 0);
	
	std::string	line1{" __   __  __   __         _______  __   __         _______  ______           _______  _______  _______  _______ "};
	std::string	line2{"|  |_|  ||  | |  |       |       ||  | |  |       |       ||    _ |         |       ||  _    ||  _    ||  _    |"};
	std::string	line3{"|       ||  | |  | ____  |_     _||  |_|  | ____  |    ___||   | ||   ____  |___    || | |   || | |   || | |   |"};
	std::string	line4{"|       ||  |_|  ||____|   |   |  |       ||____| |   |___ |   |_||_ |____|  ___|   || | |   || | |   || | |   |"};
	std::string	line5{"|       ||       |         |   |  |       |       |    ___||    __  |       |___    || |_|   || |_|   || |_|   |"};
	std::string	line6{"| ||_|| ||       |         |   |  |   _   |       |   |___ |   |  | |        ___|   ||       ||       ||       |"};
	std::string	line7{"|_|   |_||_______|         |___|  |__| |__|       |_______||___|  |_|       |_______||_______||_______||_______|"};

	mvwprintw(stdscr, 1, 4, line1.c_str());
	mvwprintw(stdscr, 2, 4, line2.c_str());
	mvwprintw(stdscr, 3, 4, line3.c_str());
	mvwprintw(stdscr, 4, 4, line4.c_str());
	mvwprintw(stdscr, 5, 4, line5.c_str());
	mvwprintw(stdscr, 6, 4, line6.c_str());
	mvwprintw(stdscr, 7, 4, line7.c_str());

	wrefresh(stdscr);
	wrefresh(chatborder);
	wrefresh(inputborder);
}

bool handle_input(WINDOW *win, std::string& outstr)
{
	int ch;
	static std::string str;
	static auto str_it = str.end();
	static int pos{0};
	if( ch != ERR )
	{	
		ch = getch();
		if(ch != ERR)
		{
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
				outstr = str;
				str.clear();
				pos = 0;
				str_it = str.end();
				wdeleteln(win);
				wmove(win, 2, 2);
				return true;
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
				break;
			}
		}
	}
	return false;
}

}