# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2019.3.2\bin\cmake\win\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2019.3.2\bin\cmake\win\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/SPADE_PROJ.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SPADE_PROJ.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SPADE_PROJ.dir/flags.make

CMakeFiles/SPADE_PROJ.dir/main.c.obj: CMakeFiles/SPADE_PROJ.dir/flags.make
CMakeFiles/SPADE_PROJ.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/SPADE_PROJ.dir/main.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\SPADE_PROJ.dir\main.c.obj   -c C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\main.c

CMakeFiles/SPADE_PROJ.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SPADE_PROJ.dir/main.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\main.c > CMakeFiles\SPADE_PROJ.dir\main.c.i

CMakeFiles/SPADE_PROJ.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SPADE_PROJ.dir/main.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\main.c -o CMakeFiles\SPADE_PROJ.dir\main.c.s

# Object files for target SPADE_PROJ
SPADE_PROJ_OBJECTS = \
"CMakeFiles/SPADE_PROJ.dir/main.c.obj"

# External object files for target SPADE_PROJ
SPADE_PROJ_EXTERNAL_OBJECTS =

SPADE_PROJ.exe: CMakeFiles/SPADE_PROJ.dir/main.c.obj
SPADE_PROJ.exe: CMakeFiles/SPADE_PROJ.dir/build.make
SPADE_PROJ.exe: CMakeFiles/SPADE_PROJ.dir/linklibs.rsp
SPADE_PROJ.exe: CMakeFiles/SPADE_PROJ.dir/objects1.rsp
SPADE_PROJ.exe: CMakeFiles/SPADE_PROJ.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable SPADE_PROJ.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\SPADE_PROJ.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SPADE_PROJ.dir/build: SPADE_PROJ.exe

.PHONY : CMakeFiles/SPADE_PROJ.dir/build

CMakeFiles/SPADE_PROJ.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\SPADE_PROJ.dir\cmake_clean.cmake
.PHONY : CMakeFiles/SPADE_PROJ.dir/clean

CMakeFiles/SPADE_PROJ.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug C:\Users\Kev\Documents\UNI\VARSAVIA\Datamining\SPADE-PROJ\cmake-build-debug\CMakeFiles\SPADE_PROJ.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SPADE_PROJ.dir/depend

