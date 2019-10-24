cmake_minimum_required(VERSION 3.15)
project(Dominoes_over_TCP)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dominoes_over_TCP_Server mainServer.cpp constants.h Socket.h ServerSocket.cpp ServerSocket.h DominoesServer.cpp DominoesServer.h User.h UsersManager.cpp UsersManager.h Domino.h Domino.cpp)
add_executable(Dominoes_over_TCP_Client mainClient.cpp constants.h Socket.h ClientSocket.cpp ClientSocket.h)
