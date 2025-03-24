cmake_minimum_required(VERSION 3.10)

project(weenystring)

add_library(weenystring INTERFACE)
target_include_directories(weenystring INTERFACE ./include)

add_subdirectory(googletest)
add_subdirectory(test)
