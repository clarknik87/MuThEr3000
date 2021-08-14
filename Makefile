# Makefile for the socket programming example
#

compiler_flags = -std=c++14

simple_server_objects = ServerSocket.o Socket.o simple_server_main.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o


all : simple_server simple_client

simple_server: $(simple_server_objects)
	g++ $(compiler_flags) -o simple_server $(simple_server_objects)


simple_client: $(simple_client_objects)
	g++ $(compiler_flags) -o simple_client $(simple_client_objects)


Socket: Socket.cpp
ServerSocket: ServerSocket.cpp
ClientSocket: ClientSocket.cpp
simple_server_main: simple_server_main.cpp
simple_client_main: simple_client_main.cpp


clean:
	rm -f *.o simple_server simple_client