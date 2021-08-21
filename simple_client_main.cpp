#include "ClientSocket.h"
#include "SocketException.h"
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
		
		while(!quitReadThread)
		{
			(*socket) >> socket_data;
			lk.lock();
			chatqueue.push(socket_data);
			lk.unlock();
		}
	}
}

void mutherClient(ClientSocket &socket)
{
	std::string	rdata;
	std::string sdata;
	
	//Initial client/server handshake
	socket >> rdata;
	if( rdata == "Connecting to client" )
	{
		std::cout << "Terminal connected \n\n\n" << std::endl;
	}
	
	socket << "Client connected";
	
	// Start thread for reading incoming messages
	std::thread readthread(readSocket, &socket);
	
	// Read and print client side data
	std::unique_lock<std::mutex> lk(queuem, std::defer_lock);
	while( rdata != ":q" )
	{
		const auto start_t = std::chrono::steady_clock::now();
		lk.lock();
		if( !chatqueue.empty() )
		{
			rdata = chatqueue.front();
			chatqueue.pop();
			std::cout << rdata;
		}
		lk.unlock();
		std::this_thread::sleep_until( start_t + 40ms ); //25 fps
	}
	
	quitReadThread = true;
	readthread.join();
	std::cout << "Terminating connection" << std::endl;
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
