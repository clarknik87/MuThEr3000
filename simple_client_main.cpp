#include "ClientSocket.h"
#include "SocketException.h"
#include <iostream>
#include <string>


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
	
	// Main chat routine
	bool stateWaiting{true};
	
	while( sdata != ":q" )
	{
		if( stateWaiting )
		{
			socket >> rdata;
			std::cout << "Muther:\t\t" << rdata;
			stateWaiting = false;
		}
		else
		{
			sdata.clear();
			std::cout << "Terminal:\t";
			while( sdata.empty() )
			{
				std::getline(std::cin, sdata);
			}
			socket << sdata  << "\n";
			stateWaiting = true;
		}
	}
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
