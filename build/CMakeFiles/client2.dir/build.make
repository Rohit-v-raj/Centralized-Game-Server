# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rohit/CLionProjects/Tictactoe

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rohit/CLionProjects/Tictactoe/build

# Include any dependencies generated for this target.
include CMakeFiles/client2.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/client2.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/client2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/client2.dir/flags.make

CMakeFiles/client2.dir/client.cpp.o: CMakeFiles/client2.dir/flags.make
CMakeFiles/client2.dir/client.cpp.o: ../client.cpp
CMakeFiles/client2.dir/client.cpp.o: CMakeFiles/client2.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/rohit/CLionProjects/Tictactoe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/client2.dir/client.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/client2.dir/client.cpp.o -MF CMakeFiles/client2.dir/client.cpp.o.d -o CMakeFiles/client2.dir/client.cpp.o -c /home/rohit/CLionProjects/Tictactoe/client.cpp

CMakeFiles/client2.dir/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/client2.dir/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/rohit/CLionProjects/Tictactoe/client.cpp > CMakeFiles/client2.dir/client.cpp.i

CMakeFiles/client2.dir/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/client2.dir/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/rohit/CLionProjects/Tictactoe/client.cpp -o CMakeFiles/client2.dir/client.cpp.s

# Object files for target client2
client2_OBJECTS = \
"CMakeFiles/client2.dir/client.cpp.o"

# External object files for target client2
client2_EXTERNAL_OBJECTS =

client2: CMakeFiles/client2.dir/client.cpp.o
client2: CMakeFiles/client2.dir/build.make
client2: CMakeFiles/client2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/rohit/CLionProjects/Tictactoe/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable client2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/client2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/client2.dir/build: client2
.PHONY : CMakeFiles/client2.dir/build

CMakeFiles/client2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/client2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/client2.dir/clean

CMakeFiles/client2.dir/depend:
	cd /home/rohit/CLionProjects/Tictactoe/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/rohit/CLionProjects/Tictactoe /home/rohit/CLionProjects/Tictactoe /home/rohit/CLionProjects/Tictactoe/build /home/rohit/CLionProjects/Tictactoe/build /home/rohit/CLionProjects/Tictactoe/build/CMakeFiles/client2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/client2.dir/depend

