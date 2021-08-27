# Makefile for the socket programming example
#

CXX = g++
CXXFLAGS = -std=c++14
LDFLAGS = -lcurses -lpthread

simple_server_objects = ServerSocket.o Socket.o simple_server_main.o cursesWrapper.o
simple_client_objects = ClientSocket.o Socket.o simple_client_main.o cursesWrapper.o


all : simple_server simple_client

simple_server: $(simple_server_objects)
	g++ $(CXXFLAGS) -o simple_server $(simple_server_objects) $(LDFLAGS)


simple_client: $(simple_client_objects)
	g++ $(CXXFLAGS) -o simple_client $(simple_client_objects) $(LDFLAGS)


Socket: Socket.cpp Socket.h
ServerSocket: ServerSocket.cpp SocketException.h ServerSocket.h
ClientSocket: ClientSocket.cpp SocketException.h ClientSocket.h
simple_server_main: simple_server_main.cpp cursesWrapper.hpp SocketException.h ServerSocket.h
simple_client_main: simple_client_main.cpp cursesWrapper.hpp SocketException.h ClientSocket.h
cursesWrapper: cursesWrapper.cpp cursesWrapper.hpp


clean:
	rm -f *.o simple_server simple_client
