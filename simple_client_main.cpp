#include "ClientSocket.h"
#include "SocketException.h"
#include "cursesWrapper.hpp"
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>

using namespace std::chrono_literals;

std::queue<std::string> chatqueue;
std::mutex queuem;
bool quitReadThread{false};

void readSocket(ClientSocket *socket)
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

void mutherClient(ClientSocket &socket)
{	
	//Initial client/server handshake
	std::string	rdata;
	socket >> rdata;
	if( rdata != "Connecting to client" )
	{
		std::cerr << "Server-Client Handshake failed" << std::endl;
	}
	else
	{
		socket << "Client connected";
	}
	
	// Start thread for reading incoming messages
	std::thread readthread(readSocket, &socket);
	
	// Start ncurses environment
	WINDOW *chatwin;
	WINDOW *inputwin;
	CursesWrapper::start_curses();
	
	//configure windows
	int sizex, sizey;
	getmaxyx(stdscr, sizey, sizex);
	
	//Place interior windows
	chatwin = 	newwin(sizey-26, sizex-4, 10, 2);
	inputwin =	newwin(10, sizex-4, sizey-12, 2);
	
	if( chatwin == nullptr || inputwin == nullptr )
	{
		std::cerr << "Error creating internal windows" << std::endl;
		exit(1);
	}
	else
	{
		scrollok(chatwin, true);
		keypad(inputwin, true);
		nodelay(inputwin, true);
	}
	
	// Read and print client side data
	std::unique_lock<std::mutex> lk(queuem, std::defer_lock);
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
			wprintw(chatwin, "    MU-TH-ER: %s", rdata.c_str());
		}
		lk.unlock();
		
		// Get any new character data from user
		
		// Refresh all screens
		refresh();
		wrefresh(chatwin);
		wrefresh(inputwin);
		
		std::this_thread::sleep_until( start_t + 40ms ); //25 fps
	}
	
	quitReadThread = true;
	readthread.join();
	
	// End curses environment
	//delwin(chatwin);
	//delwin(inputwin);
	endwin();
    refresh();
}

int main ( int argc, char* argv[] )
{
  try
    {

		ClientSocket client_socket ( "localhost", 30000 );

		try
		{
			mutherClient(client_socket);
		}
		catch ( SocketException& ) {}
    }
	catch ( SocketException& e )
    {
      std::cout << "Exception was caught:" << e.description() << "\n";
    }

  return 0;
}
