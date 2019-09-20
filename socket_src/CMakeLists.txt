cmake_minimum_required(VERSION 3.14)
project(socket_test)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(socket_test
    ClientSocket.h
    Endpoint.cpp
    Endpoint.h
    Pi2.cpp
    Socket.cpp
    Socket.h
    UDPSocket.cpp
    UDPSocket.h)
