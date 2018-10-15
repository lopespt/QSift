cmake_minimum_required(VERSION 3.2.2) # CMake version check
project(simple_example )               # Create project "simple_example"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # Enable c++11 standard
message(STATUS ${CMAKE_CXX_COMPILER})

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Concurrent)
message(STATUS ${Qt5Core_VERSION})
message(STATUS ${OpenCV_VERSION})
message(STATUS ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS ${OpenCV_LIBRARIES})
set(CMAKE_AUTOMOC ON)

include_directories(${OpenCV_INCLUDE_DIRECTORIES} include ${Boost_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})

set(SOURCE_FILES
        src/VideoSurf.cpp include/VideoSurf.hpp) # Add main.cpp file of project root directory as source file

# add_library(sift ${SOURCE_FILES})
# target_link_libraries(${OpenCV_LIBRARIES})


file(GLOB_RECURSE SRC_UTILITIES src/*.cpp)
add_library(utilities ${SRC_UTILITIES})
target_link_libraries(utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES})


SET(EXP_SRC Experimentos/)
add_executable(OrdenacaoSurf ${EXP_SRC}/OrdenacaoSurf/main.cpp)
target_link_libraries(OrdenacaoSurf utilities ${OpenCV_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})


SET(EXP_SRC Experimentos/)
add_executable(TestSurf ${EXP_SRC}/TestSurf/main.cpp)
target_link_libraries(TestSurf utilities ${OpenCV_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

SET(EXP_SRC Experimentos/)
add_executable(TestSurf2 ${EXP_SRC}/TestSurf2/main.cpp)
target_link_libraries(TestSurf2 utilities ${OpenCV_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

