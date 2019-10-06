cmake_minimum_required(VERSION 3.15)
project(Dominoes_over_TCP)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dominoes_over_TCP_Server mainServer.cpp)
add_executable(Dominoes_over_TCP_Client_1 mainClient.cpp)
add_executable(Dominoes_over_TCP_Client_2 mainClient.cpp)
