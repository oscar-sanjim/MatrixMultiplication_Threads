cmake_minimum_required(VERSION 3.12)
project(MatrixMultiplication_Threads)

set(CMAKE_CXX_STANDARD 14)

find_package( Threads )

set(SOURCES main.c matrix.c FileIO.c matrix.h FileIO.h)

add_executable(MatrixMultiplication_Threads ${SOURCES})

target_include_directories(MatrixMultiplication_Threads PRIVATE ${.})

target_link_libraries( MatrixMultiplication_Threads ${CMAKE_THREAD_LIBS_INIT} )

configure_file(test.txt test.txt COPYONLY)