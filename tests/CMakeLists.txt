cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(tests
	main.cpp
	LifeHashTreeTest.cpp 
	../src/model/LifeHashTree.hpp
	../src/model/LifeHashTree.cpp
	vectorgridtest.cpp
)