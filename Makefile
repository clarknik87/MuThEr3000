# Makefile for the socket programming example
#

compiler_flags = -std=c++14
linker_flags = -lcurses

simple_server_objects = ServerSocket.o Socket.o simple_server_main.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o


all : simple_server simple_client

simple_server: $(simple_server_objects)
	g++ $(compiler_flags) -o simple_server $(simple_server_objects) $(linker_flags)


simple_client: $(simple_client_objects)
	g++ $(compiler_flags) -o simple_client $(simple_client_objects) $(linker_flags)


Socket: Socket.cpp Socket.h
ServerSocket: ServerSocket.cpp SocketException.h ServerSocket.h
ClientSocket: ClientSocket.cpp SocketException.h ClientSocket.h
simple_server_main: simple_server_main.cpp cursesWrapper.hpp SocketException.h ServerSocket.h
simple_client_main: simple_client_main.cpp cursesWrapper.hpp SocketException.h ClientSocket.h


clean:
	rm -f *.o simple_server simple_client