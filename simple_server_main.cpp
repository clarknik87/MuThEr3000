#include "ServerSocket.h"
#include "SocketException.h"
#include "cursesWrapper.hpp"
#include <string>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>

using namespace std::chrono_literals;

std::queue<std::string> chatqueue;
std::mutex queuem;
bool quitReadThread{false};

void readSocket(ServerSocket *socket)
{
	if(socket != nullptr)
	{
		std::string socket_data;
		std::unique_lock<std::mutex> lk(queuem, std::defer_lock);
		bool socketTerminated{false};
		
		while(!quitReadThread && !socketTerminated)
		{
			(*socket) >> socket_data;
			lk.lock();
			chatqueue.push(socket_data);
			lk.unlock();
			if( socket_data == ":q\n" )
			{
				socketTerminated = true;
			}
		}
	}
}

void mutherServer(ServerSocket &socket)
{
	std::string	rdata;
	std::string sdata;
	
	// Initial client/server handshake
	std::cout << "Establishing connection..." << std::endl;
	socket << "Connecting to client";
	socket >> sdata;
	if( sdata != "Client connected" )
	{
		std::cerr << "Server-Client Handshake failed" << std::endl;
		exit(1);
	}

	// Start thread for reading incoming messages
	std::thread readthread(readSocket, &socket);
	
	// Start ncurses environment
	WINDOW *chatwin;
	WINDOW *chatborder;
	WINDOW *inputwin;
	WINDOW *inputborder;
	CursesWrapper::start_curses();
	
	//configure windows
	int sizex, sizey;
	getmaxyx(stdscr, sizey, sizex);
	
	//Place interior windows
	chatwin 	= 	newwin(sizey-26, sizex-6, 10, 3);
	chatborder 	= 	newwin(sizey-24, sizex-4, 9, 2);
	inputwin 	=	newwin(10, sizex-8, sizey-12, 5);
	inputborder =	newwin(12, sizex-4, sizey-13, 2);
	
	if( chatwin == nullptr || inputwin == nullptr )
	{
		std::cerr << "Error creating internal windows" << std::endl;
		exit(1);
	}
	
	scrollok(chatwin, true);
	scrollok(inputwin, true);
	scrollok(stdscr, true);
	keypad(stdscr, true);
	keypad(inputwin, true);
	nodelay(stdscr, true);
	nodelay(inputwin, true);
	
	
	// Read and print client side data
	std::unique_lock<std::mutex> lk(queuem, std::defer_lock);
	
	// Move cursor to starting position
	CursesWrapper::draw_borders(chatborder, inputborder);
	wmove(inputwin, 2, 0);
	wrefresh(inputwin);
	
	// Main loop
	int i=0;
	while( rdata != ":q\n" )
	{
		const auto start_t = std::chrono::steady_clock::now();
		
		// Print any new data from server
		lk.lock();
		if( !chatqueue.empty() )
		{
			rdata = chatqueue.front();
			chatqueue.pop();
			wprintw(chatwin, "    Terminal: %s\n", rdata.c_str());
		}
		lk.unlock();
		
		// Get any new character data from user
		if( CursesWrapper::handle_input(inputwin, sdata) == true )
		{
			socket << sdata;
			wprintw(chatwin, "    MU-TH-ER: %s\n", sdata.c_str());
		}
		
		// Refresh all screens
		wrefresh(stdscr);
		wrefresh(chatwin);
		wrefresh(inputwin);
		
		std::this_thread::sleep_until( start_t + 40ms ); //25 fps
	}
	
	quitReadThread = true;
	readthread.join();
	
	// End curses environment
	delwin(chatwin);
	delwin(inputwin);
	endwin();
}




int main ( int argc, char* argv[] )
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server ( 1416 );

      while ( true )
	{

	  ServerSocket new_sock;
	  server.accept ( new_sock );

	  try
	    {
	      mutherServer(new_sock);
		  break;
	    }
	  catch ( SocketException& ) {}

	}
    }
  catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\nExiting.\n";
    }

  return 0;
}
