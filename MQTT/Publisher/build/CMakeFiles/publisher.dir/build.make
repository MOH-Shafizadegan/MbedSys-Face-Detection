# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build"

# Include any dependencies generated for this target.
include CMakeFiles/publisher.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/publisher.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/publisher.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/publisher.dir/flags.make

CMakeFiles/publisher.dir/src/Publisher.cpp.o: CMakeFiles/publisher.dir/flags.make
CMakeFiles/publisher.dir/src/Publisher.cpp.o: ../src/Publisher.cpp
CMakeFiles/publisher.dir/src/Publisher.cpp.o: CMakeFiles/publisher.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/publisher.dir/src/Publisher.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/publisher.dir/src/Publisher.cpp.o -MF CMakeFiles/publisher.dir/src/Publisher.cpp.o.d -o CMakeFiles/publisher.dir/src/Publisher.cpp.o -c "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/src/Publisher.cpp"

CMakeFiles/publisher.dir/src/Publisher.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/publisher.dir/src/Publisher.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/src/Publisher.cpp" > CMakeFiles/publisher.dir/src/Publisher.cpp.i

CMakeFiles/publisher.dir/src/Publisher.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/publisher.dir/src/Publisher.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/src/Publisher.cpp" -o CMakeFiles/publisher.dir/src/Publisher.cpp.s

# Object files for target publisher
publisher_OBJECTS = \
"CMakeFiles/publisher.dir/src/Publisher.cpp.o"

# External object files for target publisher
publisher_EXTERNAL_OBJECTS =

bin/publisher: CMakeFiles/publisher.dir/src/Publisher.cpp.o
bin/publisher: CMakeFiles/publisher.dir/build.make
bin/publisher: CMakeFiles/publisher.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bin/publisher"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/publisher.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/publisher.dir/build: bin/publisher
.PHONY : CMakeFiles/publisher.dir/build

CMakeFiles/publisher.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/publisher.dir/cmake_clean.cmake
.PHONY : CMakeFiles/publisher.dir/clean

CMakeFiles/publisher.dir/depend:
	cd "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher" "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher" "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build" "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build" "/media/mohsh/EDU/SUT/Semester6/Embedded Systems/Practice/Face Detection/Code/MQTT/Publisher/build/CMakeFiles/publisher.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/publisher.dir/depend

