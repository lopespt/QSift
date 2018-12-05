CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

# Author : Pierre Moulon
# Date : December 2010.
# README :
#          The following cmake works for windows.
#	Openmp is accessible only on the professionnal version of Visual Studio.
# 	In order to use OPENMP in visual your have to add the preprocessor _OPENMP
# 	and enable OPENMP library in C/C++/Language.
PROJECT(ASIFT)

#ADD_SUBDIRECTORY(io_png)
ADD_SUBDIRECTORY(libMatch)
ADD_SUBDIRECTORY(libNumerics)

FIND_PACKAGE(PNG)
FIND_PACKAGE(ZLIB)
MESSAGE(STATUS ${ZLIB_LIBRARIES})

FIND_PACKAGE(OpenMP) 
if (OPENMP_FOUND) 
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif (OPENMP_FOUND)

IF(MSVC)
   ADD_DEFINITIONS(/arch:SSE2)
ENDIF(MSVC)

IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-strict-aliasing")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wno-write-strings")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated -ansi")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ftree-vectorize -funroll-loops")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)


set(ASIFT_srcs
	numerics1.cpp frot.cpp splines.cpp fproj.cpp
	library.cpp flimage.cpp filter.cpp
	demo_lib_sift.cpp compute_asift_keypoints.cpp
	compute_asift_matches.cpp
	orsa.cpp
	io_png/io_png.c)
	

	
add_library(asift numerics1.cpp frot.cpp splines.cpp fproj.cpp
library.cpp flimage.cpp filter.cpp compute_asift_keypoints.cpp
compute_asift_matches.cpp orsa.cpp demo_lib_sift.cpp io_png/io_png.c)
TARGET_LINK_LIBRARIES(asift PRIVATE Match Numerics) 
TARGET_LINK_LIBRARIES(asift PUBLIC ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})

target_include_directories(asift PRIVATE .)
					#./io_png
					#./io_png/libs/png)

#add_executable(demo_ASIFT ${ASIFT_srcs})
#target_include_directories(demo_ASIFT PRIVATE .)
					#./io_png
					#./io_png/libs/png)
#TARGET_LINK_LIBRARIES(demo_ASIFT PUBLIC ${ZLIB_LIBRARIES} ${PNG_LIBRARIES} Match Numerics)
