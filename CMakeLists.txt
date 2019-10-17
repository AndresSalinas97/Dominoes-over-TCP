cmake_minimum_required(VERSION 3.15)
project(Dominoes_over_TCP)

set(CMAKE_CXX_STANDARD 14)

add_executable(Dominoes_over_TCP_Server mainServer.cpp serverUtils.h serverUtils.cpp constants.h Socket.cpp Socket.h)
add_executable(Dominoes_over_TCP_Client mainClient.cpp Socket.cpp Socket.h)
