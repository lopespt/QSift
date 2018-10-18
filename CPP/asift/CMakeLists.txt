cmake_minimum_required(VERSION 3.2.2) # CMake version check
project(simple_example )               # Create project "simple_example"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # Enable c++11 standard
message(STATUS ${CMAKE_CXX_COMPILER})


IF(MSVC)
   ADD_DEFINITIONS(/arch:SSE2)
ENDIF(MSVC)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-strict-aliasing")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-write-strings")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -ansi")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -funroll-loops")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
 
message(STATUS ${Qt5Core_VERSION})
message(STATUS ${OpenCV_VERSION})
message(STATUS ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS ${OpenCV_LIBRARIES})
set(CMAKE_AUTOMOC ON)



add_subdirectory(src/)
find_package(OpenCV REQUIRED)


include_directories(${OpenCV_INCLUDE_DIRECTORIES} src include ${Boost_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})


file(GLOB_RECURSE SRC_UTILITIES src2/*.cpp)
add_library(utilities ${SRC_UTILITIES})
target_link_libraries(utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES})



SET(EXP_SRC Experimentos/)
add_executable(OrdenacaoASift ${EXP_SRC}/OrdenacaoASift/main.cpp)
target_link_libraries(OrdenacaoASift utilities asift ${OpenCV_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})


#[==[ 
FIND_PACKAGE(OpenMP) 
if (OPENMP_FOUND) 
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif (OPENMP_FOUND)
#]==]
