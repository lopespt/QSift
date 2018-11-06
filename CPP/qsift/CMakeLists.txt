cmake_minimum_required(VERSION 3.2.2) # CMake version check
project(simple_example )               # Create project "simple_example"

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -O3") # Enable c++11 standard
message(STATUS ${CMAKE_CXX_COMPILER})

FIND_PACKAGE(OpenCV REQUIRED)
FIND_PACKAGE(Boost REQUIRED)
FIND_PACKAGE(Qt5Core)
FIND_PACKAGE(Qt5Concurrent)
#FIND_PACKAGE(OpenCL REQUIRED)
#FIND_PACKAGE(Matlab COMPONENTS ENG_LIBRARY MEX_LIBRARY MX_LIBRARY)
#message(STATUS ${OpenCL_LIBRARIES})
#message(STATUS ${Matlab_INCLUDE_DIRS})
#message(STATUS ${Matlab_ENG_LIBRARY})
message(STATUS ${Qt5Core_VERSION})
message(STATUS ${OpenCV_VERSION})
message(STATUS ${CMAKE_CXX_COMPILER_VERSION})
message(STATUS ${OpenCV_LIBRARIES})
set(CMAKE_AUTOMOC ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(${OpenCV_INCLUDE_DIRECTORIES} include include2 ${Boost_INCLUDE_DIRS} ${OpenCL_INCLUDE_DIRS} ${Matlab_INCLUDE_DIRS} ${Qt5Core_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})


set(SOURCE_FILES
        src/dspfeat.c
        src/imgfeatures.c
        src/kdtree.c
        src/match.c
        src/minpq.c
        src/sift.c
        src/siftfeat.c
        src/utils.c
        src/xform.c
        src2/VideoSift.cpp include2/VideoSift.hpp src2/tools.cpp include2/tools.hpp)            # Add main.cpp file of project root directory as source file

add_library(sift ${SOURCE_FILES})
target_link_libraries(sift ${OpenCV_LIBRARIES})


file(GLOB_RECURSE SRC_UTILITIES src2/*.cpp)
add_library(utilities ${SRC_UTILITIES} src2/tools.cpp include2/tools.hpp)
target_link_libraries(utilities sift  ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES})


SET(EXP_SRC Experimentos/)
add_executable(Experimento1 ${EXP_SRC}/Exp1/main.cpp)
target_link_libraries(Experimento1 sift utilities ${OpenCV_LIBRARIES})

add_executable(Experimento2 ${EXP_SRC}/Exp2/main.cpp)
target_link_libraries(Experimento2 sift utilities ${OpenCV_LIBRARIES})


add_executable(CreateSceneSequence ${EXP_SRC}/CreateSceneSequences/main.cpp)
target_link_libraries(CreateSceneSequence ${OpenCV_LIBRARIES} sift utilities  ${OpenCL_LIBRARIES})


add_executable(TestPDM ${EXP_SRC}/TestPDM/main.cpp)
target_link_libraries(TestPDM sift utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES})


add_executable(OrdenacaoSift ${EXP_SRC}/OrdenacaoSift/main.cpp)
target_link_libraries(OrdenacaoSift sift utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} ${Matlab_LIBRARIES})

add_executable(GaussianTest ${EXP_SRC}/GaussianTest/main.cpp)
target_link_libraries(GaussianTest utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} ${Matlab_LIBRARIES})

add_executable(OrdenacaoSift2 ${EXP_SRC}/OrdenacaoSift2/main.cpp)
target_link_libraries(OrdenacaoSift2 sift utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} ${Matlab_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

add_executable(Conversor ${EXP_SRC}/Conversor/main.cpp)
target_link_libraries(Conversor  ${OpenCV_LIBRARIES}  ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

add_executable(OpenMP ${EXP_SRC}/OpenMP/main.cpp)
target_link_libraries(OpenMP ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})

add_executable(FrameMatching ${EXP_SRC}/FrameMatching/main.cpp)
target_link_libraries(FrameMatching sift utilities ${OpenCV_LIBRARIES} ${OpenCL_LIBRARIES} ${Matlab_LIBRARIES} ${Qt5Core_LIBRARIES} ${Qt5Concurrent_LIBRARIES})
