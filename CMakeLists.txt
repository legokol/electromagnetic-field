cmake_minimum_required(VERSION 3.20)
project(electromagnetic-field)

add_library(${PROJECT_NAME})
add_subdirectory(include)
add_subdirectory(src)

add_subdirectory(examples)
