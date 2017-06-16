cmake_minimum_required(VERSION 3.7)
project(CarSim)

find_package(Curses REQUIRED)
find_package(Threads)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp car.cpp map.cpp vec2.cpp toolbox.cpp config.cpp graph.cpp)
set(HEADER_FILES car.hpp map.hpp config.hpp graph.hpp)

add_executable(CarSim ${SOURCE_FILES} ${HEADER_FILES})
target_link_libraries(CarSim ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
