cmake_minimum_required(VERSION 3.3)

project(SeaSalt)

add_executable(sea_salt 
	src/main.cpp
	src/tokens.hpp
	src/ast.hpp
)
