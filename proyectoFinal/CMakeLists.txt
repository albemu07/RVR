cmake_minimum_required(VERSION 3.0.0)
project(project VERSION 0.1.0)

add_executable(project src/main.cpp)

target_link_libraries(project SDL2main SDL2 GL)
