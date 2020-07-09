cmake_minimum_required(VERSION 2.8)
project(traffic)

set(SOURCES traffic/util.cpp traffic/car.cpp traffic/traffic.cpp main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -std=c++1z")

add_executable(${PROJECT_NAME} ${SOURCES})