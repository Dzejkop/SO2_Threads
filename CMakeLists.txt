cmake_minimum_required(VERSION 3.7)
project(CarSim)

find_package(Curses REQUIRED)
find_package (Threads)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp car.cpp car.hpp map.cpp map.hpp vec2.cpp vec2.hpp common.hpp toolbox.hpp toolbox.cpp)
add_executable(CarSim ${SOURCE_FILES})
target_link_libraries(CarSim ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})