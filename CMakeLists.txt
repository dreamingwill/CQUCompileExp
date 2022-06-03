cmake_minimum_required(VERSION 3.22)
project(proj3)

set(CMAKE_CXX_STANDARD 14)

add_executable(proj3 main.cpp Token.h lex.h parse.h Token.cpp lex.cpp parse.cpp Idenfr.cpp Idenfr.h)
