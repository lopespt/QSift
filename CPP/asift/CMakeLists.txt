cmake_minimum_required(VERSION 3.2.2) # CMake version check
project(simple_example )               # Create project "simple_example"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17") # Enable c++11 standard
message(STATUS ${CMAKE_CXX_COMPILER})

FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Concurrent) 
FIND_PACKAGE(OpenMP) 
FIND_PACKAGE(Boost REQUIRED COMPONENTS serialization)
add_library(boost::serialization INTERFACE IMPORTED)
  set_target_properties(boost::serialization PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${Boost_INCLUDE_DIRS}"
    INTERFACE_LINK_LIBRARIES "${Boost_SERIALIZATION_LIBRARIES}")

if (OPENMP_FOUND) 
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif (OPENMP_FOUND)

SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF(MSVC)
   ADD_DEFINITIONS(/arch:SSE2)
ENDIF(MSVC)

#Resolve problemas de linker com o cblas
set(CMAKE_EXE_LINKER_FLAGS -lcblas)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -Wno-strict-aliasing")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-write-strings")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ftree-vectorize -funroll-loops -O3")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)
 
message(STATUS ${Qt5Core_VERSION})
message(STATUS ${OpenCV_VERSION})
message(STATUS ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS ${OpenCV_LIBRARIES})
set(CMAKE_AUTOMOC ON)



add_subdirectory(src/)
find_package(OpenCV REQUIRED)





file(GLOB_RECURSE SRC_UTILITIES src2/*.cpp)
add_library(utilities ${SRC_UTILITIES})
target_link_libraries(utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} boost::serialization)
target_include_directories(utilities PUBLIC ${OpenCV_INCLUDE_DIRECTORIES} src include ${Boost_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})



SET(EXP_SRC Experimentos/)
add_executable(OrdenacaoASift ${EXP_SRC}/OrdenacaoASift/main.cpp)
message(STATUS teste)
message(STATUS ${Qt5Core_INCLUDE_DIRS})
message(STATUS "t")
target_include_directories(OrdenacaoASift PUBLIC ${Qt5Core_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})
target_link_libraries(OrdenacaoASift cblas utilities asift ${OpenMP_LIBRARIES} ${OpenCV_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES} boost::serialization)


