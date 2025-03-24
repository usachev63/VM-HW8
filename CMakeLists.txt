cmake_minimum_required(VERSION 3.10)

project(weenystring)

add_library(weenystring src/WeenyString.cpp)
target_include_directories(weenystring PUBLIC ./include)

add_subdirectory(googletest)
add_subdirectory(test)
