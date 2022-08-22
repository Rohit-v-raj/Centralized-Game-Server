cmake_minimum_required(VERSION 3.21)
project(Tictactoe)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(client client.cpp tictactoe.h)
add_executable(client1 client.cpp tictactoe.h)
add_executable(client2 client.cpp tictactoe.h)
add_executable(client3 client.cpp tictactoe.h)
add_executable(server server.cpp server.h)

target_link_libraries(server ${CMAKE_THREAD_LIBS_INIT})


