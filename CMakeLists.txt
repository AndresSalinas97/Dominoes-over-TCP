cmake_minimum_required(VERSION 3.10)

project(Dominoes_over_TCP)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dominoes_over_TCP_Server src/mainServer.cpp src/constants.h src/Socket/Socket.h src/Socket/ServerSocket.cpp src/Socket/ServerSocket.h src/DominoesServer/DominoesServer.cpp src/DominoesServer/DominoesServer.h src/User/User.h src/User/UsersManager.cpp src/User/UsersManager.h src/DominoesGame/DominoTile.h src/DominoesGame/DominoTile.cpp src/DominoesGame/DominoesBoard.cpp src/DominoesGame/DominoesBoard.h)
add_executable(Dominoes_over_TCP_Client src/mainClient.cpp src/constants.h src/Socket/Socket.h src/Socket/ClientSocket.cpp src/Socket/ClientSocket.h)
