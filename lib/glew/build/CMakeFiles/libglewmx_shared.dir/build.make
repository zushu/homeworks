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
CMAKE_COMMAND = /Users/zumrudshukurlu/miniconda2/bin/cmake

# The command to remove a file.
RM = /Users/zumrudshukurlu/miniconda2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build

# Include any dependencies generated for this target.
include CMakeFiles/libglewmx_shared.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/libglewmx_shared.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/libglewmx_shared.dir/flags.make

CMakeFiles/libglewmx_shared.dir/src/glew.c.o: CMakeFiles/libglewmx_shared.dir/flags.make
CMakeFiles/libglewmx_shared.dir/src/glew.c.o: ../src/glew.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/libglewmx_shared.dir/src/glew.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/libglewmx_shared.dir/src/glew.c.o   -c /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/src/glew.c

CMakeFiles/libglewmx_shared.dir/src/glew.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/libglewmx_shared.dir/src/glew.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/src/glew.c > CMakeFiles/libglewmx_shared.dir/src/glew.c.i

CMakeFiles/libglewmx_shared.dir/src/glew.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/libglewmx_shared.dir/src/glew.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/src/glew.c -o CMakeFiles/libglewmx_shared.dir/src/glew.c.s

# Object files for target libglewmx_shared
libglewmx_shared_OBJECTS = \
"CMakeFiles/libglewmx_shared.dir/src/glew.c.o"

# External object files for target libglewmx_shared
libglewmx_shared_EXTERNAL_OBJECTS =

lib/libglewmx.dylib: CMakeFiles/libglewmx_shared.dir/src/glew.c.o
lib/libglewmx.dylib: CMakeFiles/libglewmx_shared.dir/build.make
lib/libglewmx.dylib: CMakeFiles/libglewmx_shared.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library lib/libglewmx.dylib"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/libglewmx_shared.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/libglewmx_shared.dir/build: lib/libglewmx.dylib

.PHONY : CMakeFiles/libglewmx_shared.dir/build

CMakeFiles/libglewmx_shared.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/libglewmx_shared.dir/cmake_clean.cmake
.PHONY : CMakeFiles/libglewmx_shared.dir/clean

CMakeFiles/libglewmx_shared.dir/depend:
	cd /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build /Users/zumrudshukurlu/Desktop/dersler20191/OpenGL_CMake_Skeleton/lib/glew/build/CMakeFiles/libglewmx_shared.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/libglewmx_shared.dir/depend

