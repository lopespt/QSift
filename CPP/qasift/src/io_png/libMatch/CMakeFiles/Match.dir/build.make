# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.12

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lopespt/tmp/qsift/CPP/asift/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lopespt/tmp/qsift/CPP/asift/src/io_png

# Include any dependencies generated for this target.
include libMatch/CMakeFiles/Match.dir/depend.make

# Include the progress variables for this target.
include libMatch/CMakeFiles/Match.dir/progress.make

# Include the compile flags for this target's objects.
include libMatch/CMakeFiles/Match.dir/flags.make

libMatch/CMakeFiles/Match.dir/match.cpp.o: libMatch/CMakeFiles/Match.dir/flags.make
libMatch/CMakeFiles/Match.dir/match.cpp.o: ../libMatch/match.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lopespt/tmp/qsift/CPP/asift/src/io_png/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object libMatch/CMakeFiles/Match.dir/match.cpp.o"
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && /home/lopespt/.vim/bin/cc_args_g++.sh  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Match.dir/match.cpp.o -c /home/lopespt/tmp/qsift/CPP/asift/src/libMatch/match.cpp

libMatch/CMakeFiles/Match.dir/match.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Match.dir/match.cpp.i"
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && /home/lopespt/.vim/bin/cc_args_g++.sh $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lopespt/tmp/qsift/CPP/asift/src/libMatch/match.cpp > CMakeFiles/Match.dir/match.cpp.i

libMatch/CMakeFiles/Match.dir/match.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Match.dir/match.cpp.s"
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && /home/lopespt/.vim/bin/cc_args_g++.sh $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lopespt/tmp/qsift/CPP/asift/src/libMatch/match.cpp -o CMakeFiles/Match.dir/match.cpp.s

# Object files for target Match
Match_OBJECTS = \
"CMakeFiles/Match.dir/match.cpp.o"

# External object files for target Match
Match_EXTERNAL_OBJECTS =

libMatch/libMatch.a: libMatch/CMakeFiles/Match.dir/match.cpp.o
libMatch/libMatch.a: libMatch/CMakeFiles/Match.dir/build.make
libMatch/libMatch.a: libMatch/CMakeFiles/Match.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lopespt/tmp/qsift/CPP/asift/src/io_png/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libMatch.a"
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && $(CMAKE_COMMAND) -P CMakeFiles/Match.dir/cmake_clean_target.cmake
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Match.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
libMatch/CMakeFiles/Match.dir/build: libMatch/libMatch.a

.PHONY : libMatch/CMakeFiles/Match.dir/build

libMatch/CMakeFiles/Match.dir/clean:
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch && $(CMAKE_COMMAND) -P CMakeFiles/Match.dir/cmake_clean.cmake
.PHONY : libMatch/CMakeFiles/Match.dir/clean

libMatch/CMakeFiles/Match.dir/depend:
	cd /home/lopespt/tmp/qsift/CPP/asift/src/io_png && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lopespt/tmp/qsift/CPP/asift/src /home/lopespt/tmp/qsift/CPP/asift/src/libMatch /home/lopespt/tmp/qsift/CPP/asift/src/io_png /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch /home/lopespt/tmp/qsift/CPP/asift/src/io_png/libMatch/CMakeFiles/Match.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : libMatch/CMakeFiles/Match.dir/depend
