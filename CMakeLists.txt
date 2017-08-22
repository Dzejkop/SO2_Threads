cmake_minimum_required(VERSION 3.0)
project(CarSim)

find_package(Curses REQUIRED)
find_package(Threads)
include_directories(${CURSES_INCLUDE_DIR})

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/sources)

include_directories("${PROJECT_SOURCE_DIR}/headers")

set(SOURCE_FILES 
    ${SOURCE_DIR}/main.cpp 
    ${SOURCE_DIR}/car.cpp 
    ${SOURCE_DIR}/map.cpp 
    ${SOURCE_DIR}/rendering.cpp 
    ${SOURCE_DIR}/config.cpp 
    ${SOURCE_DIR}/graph.cpp
    ${SOURCE_DIR}/stats.cpp
    ${SOURCE_DIR}/vec2.cpp
)

add_executable(CarSim ${SOURCE_FILES})
file(COPY ${PROJECT_SOURCE_DIR}/maps DESTINATION ${CMAKE_BINARY_DIR})
target_link_libraries(CarSim ${CURSES_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
