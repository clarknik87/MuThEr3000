#include "ServerSocket.h"
#include "SocketException.h"
#include <string>
#include <iostream>


void mutherServer(ServerSocket &socket)
{
	std::string	rdata;
	std::string sdata;
	
	// Initial client/server handshake
	std::cout << "Establishing connection..." << std::endl;
	socket << "Connecting to client";
	socket >> sdata;
	if( sdata == "Client connected" )
	{
		std::cout << "Client connected \n\n\n" << std::endl;
	}

	// Main chat routine
	
	while( sdata != ":q" )
	{
		sdata.clear();
		std::cout << "Terminal:\t";
		while( sdata.empty() )
		{
			std::getline(std::cin, sdata);
		}
		socket << sdata << "\n";
	}
	
	std::cout << "Terminating connection" << std::endl;
}




int main ( int argc, char* argv[] )
{
  std::cout << "running....\n";

  try
    {
      // Create the socket
      ServerSocket server ( 30000 );

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
