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

# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build

# Include any dependencies generated for this target.
include CMakeFiles/alloctest.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/alloctest.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/alloctest.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/alloctest.dir/flags.make

CMakeFiles/alloctest.dir/tests/alloctest.cc.o: CMakeFiles/alloctest.dir/flags.make
CMakeFiles/alloctest.dir/tests/alloctest.cc.o: ../tests/alloctest.cc
CMakeFiles/alloctest.dir/tests/alloctest.cc.o: CMakeFiles/alloctest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/alloctest.dir/tests/alloctest.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alloctest.dir/tests/alloctest.cc.o -MF CMakeFiles/alloctest.dir/tests/alloctest.cc.o.d -o CMakeFiles/alloctest.dir/tests/alloctest.cc.o -c /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/alloctest.cc

CMakeFiles/alloctest.dir/tests/alloctest.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alloctest.dir/tests/alloctest.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/alloctest.cc > CMakeFiles/alloctest.dir/tests/alloctest.cc.i

CMakeFiles/alloctest.dir/tests/alloctest.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alloctest.dir/tests/alloctest.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/alloctest.cc -o CMakeFiles/alloctest.dir/tests/alloctest.cc.s

CMakeFiles/alloctest.dir/tests/test_mark.cc.o: CMakeFiles/alloctest.dir/flags.make
CMakeFiles/alloctest.dir/tests/test_mark.cc.o: ../tests/test_mark.cc
CMakeFiles/alloctest.dir/tests/test_mark.cc.o: CMakeFiles/alloctest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/alloctest.dir/tests/test_mark.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alloctest.dir/tests/test_mark.cc.o -MF CMakeFiles/alloctest.dir/tests/test_mark.cc.o.d -o CMakeFiles/alloctest.dir/tests/test_mark.cc.o -c /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_mark.cc

CMakeFiles/alloctest.dir/tests/test_mark.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alloctest.dir/tests/test_mark.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_mark.cc > CMakeFiles/alloctest.dir/tests/test_mark.cc.i

CMakeFiles/alloctest.dir/tests/test_mark.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alloctest.dir/tests/test_mark.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_mark.cc -o CMakeFiles/alloctest.dir/tests/test_mark.cc.s

CMakeFiles/alloctest.dir/tests/test_generic.cc.o: CMakeFiles/alloctest.dir/flags.make
CMakeFiles/alloctest.dir/tests/test_generic.cc.o: ../tests/test_generic.cc
CMakeFiles/alloctest.dir/tests/test_generic.cc.o: CMakeFiles/alloctest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/alloctest.dir/tests/test_generic.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alloctest.dir/tests/test_generic.cc.o -MF CMakeFiles/alloctest.dir/tests/test_generic.cc.o.d -o CMakeFiles/alloctest.dir/tests/test_generic.cc.o -c /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_generic.cc

CMakeFiles/alloctest.dir/tests/test_generic.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alloctest.dir/tests/test_generic.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_generic.cc > CMakeFiles/alloctest.dir/tests/test_generic.cc.i

CMakeFiles/alloctest.dir/tests/test_generic.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alloctest.dir/tests/test_generic.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_generic.cc -o CMakeFiles/alloctest.dir/tests/test_generic.cc.s

CMakeFiles/alloctest.dir/tests/test_buddy.cc.o: CMakeFiles/alloctest.dir/flags.make
CMakeFiles/alloctest.dir/tests/test_buddy.cc.o: ../tests/test_buddy.cc
CMakeFiles/alloctest.dir/tests/test_buddy.cc.o: CMakeFiles/alloctest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/alloctest.dir/tests/test_buddy.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alloctest.dir/tests/test_buddy.cc.o -MF CMakeFiles/alloctest.dir/tests/test_buddy.cc.o.d -o CMakeFiles/alloctest.dir/tests/test_buddy.cc.o -c /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_buddy.cc

CMakeFiles/alloctest.dir/tests/test_buddy.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alloctest.dir/tests/test_buddy.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_buddy.cc > CMakeFiles/alloctest.dir/tests/test_buddy.cc.i

CMakeFiles/alloctest.dir/tests/test_buddy.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alloctest.dir/tests/test_buddy.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_buddy.cc -o CMakeFiles/alloctest.dir/tests/test_buddy.cc.s

CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o: CMakeFiles/alloctest.dir/flags.make
CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o: ../tests/test_run_cpp.cc
CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o: CMakeFiles/alloctest.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o -MF CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o.d -o CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o -c /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_run_cpp.cc

CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_run_cpp.cc > CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.i

CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/tests/test_run_cpp.cc -o CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.s

# Object files for target alloctest
alloctest_OBJECTS = \
"CMakeFiles/alloctest.dir/tests/alloctest.cc.o" \
"CMakeFiles/alloctest.dir/tests/test_mark.cc.o" \
"CMakeFiles/alloctest.dir/tests/test_generic.cc.o" \
"CMakeFiles/alloctest.dir/tests/test_buddy.cc.o" \
"CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o"

# External object files for target alloctest
alloctest_EXTERNAL_OBJECTS =

alloctest: CMakeFiles/alloctest.dir/tests/alloctest.cc.o
alloctest: CMakeFiles/alloctest.dir/tests/test_mark.cc.o
alloctest: CMakeFiles/alloctest.dir/tests/test_generic.cc.o
alloctest: CMakeFiles/alloctest.dir/tests/test_buddy.cc.o
alloctest: CMakeFiles/alloctest.dir/tests/test_run_cpp.cc.o
alloctest: CMakeFiles/alloctest.dir/build.make
alloctest: libensimalloc.so
alloctest: CMakeFiles/alloctest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable alloctest"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/alloctest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/alloctest.dir/build: alloctest
.PHONY : CMakeFiles/alloctest.dir/build

CMakeFiles/alloctest.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/alloctest.dir/cmake_clean.cmake
.PHONY : CMakeFiles/alloctest.dir/clean

CMakeFiles/alloctest.dir/depend:
	cd /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build /home/julian/Documentos/ENSI/Systemes/sepc/ensimag-malloc/build/CMakeFiles/alloctest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/alloctest.dir/depend

