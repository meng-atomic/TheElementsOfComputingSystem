# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.19.0_1/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.19.0_1/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build

# Include any dependencies generated for this target.
include CMakeFiles/jack_tokenizer_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/jack_tokenizer_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/jack_tokenizer_test.dir/flags.make

CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o: CMakeFiles/jack_tokenizer_test.dir/flags.make
CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o: ../jack_tokenizer_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o -c /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/jack_tokenizer_test.cpp

CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/jack_tokenizer_test.cpp > CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.i

CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/jack_tokenizer_test.cpp -o CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.s

# Object files for target jack_tokenizer_test
jack_tokenizer_test_OBJECTS = \
"CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o"

# External object files for target jack_tokenizer_test
jack_tokenizer_test_EXTERNAL_OBJECTS =

jack_tokenizer_test: CMakeFiles/jack_tokenizer_test.dir/jack_tokenizer_test.cpp.o
jack_tokenizer_test: CMakeFiles/jack_tokenizer_test.dir/build.make
jack_tokenizer_test: CMakeFiles/jack_tokenizer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable jack_tokenizer_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jack_tokenizer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/jack_tokenizer_test.dir/build: jack_tokenizer_test

.PHONY : CMakeFiles/jack_tokenizer_test.dir/build

CMakeFiles/jack_tokenizer_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/jack_tokenizer_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/jack_tokenizer_test.dir/clean

CMakeFiles/jack_tokenizer_test.dir/depend:
	cd /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build /Users/zhangmeng34/workspace/atomic/TheElementsOfComputingSystem/Chapter9-11-JackCompiler/compiler/build/CMakeFiles/jack_tokenizer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/jack_tokenizer_test.dir/depend

