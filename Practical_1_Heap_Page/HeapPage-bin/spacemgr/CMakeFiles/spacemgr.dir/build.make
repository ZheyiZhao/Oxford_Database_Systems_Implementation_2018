# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/local/Cellar/cmake/3.10.2/bin/cmake

# The command to remove a file.
RM = /usr/local/Cellar/cmake/3.10.2/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/charlottezhao/Desktop/Database/HeapPage

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/charlottezhao/Desktop/Database/HeapPage-bin

# Include any dependencies generated for this target.
include spacemgr/CMakeFiles/spacemgr.dir/depend.make

# Include the progress variables for this target.
include spacemgr/CMakeFiles/spacemgr.dir/progress.make

# Include the compile flags for this target's objects.
include spacemgr/CMakeFiles/spacemgr.dir/flags.make

spacemgr/CMakeFiles/spacemgr.dir/db.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/db.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/db.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/db.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/db.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/db.cpp

spacemgr/CMakeFiles/spacemgr.dir/db.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/db.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/db.cpp > CMakeFiles/spacemgr.dir/db.i

spacemgr/CMakeFiles/spacemgr.dir/db.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/db.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/db.cpp -o CMakeFiles/spacemgr.dir/db.s

spacemgr/CMakeFiles/spacemgr.dir/db.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/db.o.requires

spacemgr/CMakeFiles/spacemgr.dir/db.o.provides: spacemgr/CMakeFiles/spacemgr.dir/db.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/db.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/db.o.provides

spacemgr/CMakeFiles/spacemgr.dir/db.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/db.o


spacemgr/CMakeFiles/spacemgr.dir/dirpage.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/dirpage.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/dirpage.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/dirpage.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/dirpage.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/dirpage.cpp

spacemgr/CMakeFiles/spacemgr.dir/dirpage.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/dirpage.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/dirpage.cpp > CMakeFiles/spacemgr.dir/dirpage.i

spacemgr/CMakeFiles/spacemgr.dir/dirpage.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/dirpage.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/dirpage.cpp -o CMakeFiles/spacemgr.dir/dirpage.s

spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.requires

spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.provides: spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.provides

spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/dirpage.o


spacemgr/CMakeFiles/spacemgr.dir/heapfile.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/heapfile.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heapfile.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/heapfile.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/heapfile.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heapfile.cpp

spacemgr/CMakeFiles/spacemgr.dir/heapfile.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/heapfile.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heapfile.cpp > CMakeFiles/spacemgr.dir/heapfile.i

spacemgr/CMakeFiles/spacemgr.dir/heapfile.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/heapfile.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heapfile.cpp -o CMakeFiles/spacemgr.dir/heapfile.s

spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.requires

spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.provides: spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.provides

spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/heapfile.o


spacemgr/CMakeFiles/spacemgr.dir/heappage.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/heappage.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heappage.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/heappage.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/heappage.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heappage.cpp

spacemgr/CMakeFiles/spacemgr.dir/heappage.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/heappage.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heappage.cpp > CMakeFiles/spacemgr.dir/heappage.i

spacemgr/CMakeFiles/spacemgr.dir/heappage.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/heappage.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heappage.cpp -o CMakeFiles/spacemgr.dir/heappage.s

spacemgr/CMakeFiles/spacemgr.dir/heappage.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heappage.o.requires

spacemgr/CMakeFiles/spacemgr.dir/heappage.o.provides: spacemgr/CMakeFiles/spacemgr.dir/heappage.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/heappage.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heappage.o.provides

spacemgr/CMakeFiles/spacemgr.dir/heappage.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/heappage.o


spacemgr/CMakeFiles/spacemgr.dir/heaptest.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/heaptest.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heaptest.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/heaptest.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/heaptest.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heaptest.cpp

spacemgr/CMakeFiles/spacemgr.dir/heaptest.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/heaptest.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heaptest.cpp > CMakeFiles/spacemgr.dir/heaptest.i

spacemgr/CMakeFiles/spacemgr.dir/heaptest.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/heaptest.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/heaptest.cpp -o CMakeFiles/spacemgr.dir/heaptest.s

spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.requires

spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.provides: spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.provides

spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/heaptest.o


spacemgr/CMakeFiles/spacemgr.dir/page.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/page.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/page.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/page.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/page.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/page.cpp

spacemgr/CMakeFiles/spacemgr.dir/page.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/page.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/page.cpp > CMakeFiles/spacemgr.dir/page.i

spacemgr/CMakeFiles/spacemgr.dir/page.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/page.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/page.cpp -o CMakeFiles/spacemgr.dir/page.s

spacemgr/CMakeFiles/spacemgr.dir/page.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/page.o.requires

spacemgr/CMakeFiles/spacemgr.dir/page.o.provides: spacemgr/CMakeFiles/spacemgr.dir/page.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/page.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/page.o.provides

spacemgr/CMakeFiles/spacemgr.dir/page.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/page.o


spacemgr/CMakeFiles/spacemgr.dir/scan.o: spacemgr/CMakeFiles/spacemgr.dir/flags.make
spacemgr/CMakeFiles/spacemgr.dir/scan.o: /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/scan.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object spacemgr/CMakeFiles/spacemgr.dir/scan.o"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spacemgr.dir/scan.o -c /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/scan.cpp

spacemgr/CMakeFiles/spacemgr.dir/scan.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spacemgr.dir/scan.i"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/scan.cpp > CMakeFiles/spacemgr.dir/scan.i

spacemgr/CMakeFiles/spacemgr.dir/scan.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spacemgr.dir/scan.s"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr/scan.cpp -o CMakeFiles/spacemgr.dir/scan.s

spacemgr/CMakeFiles/spacemgr.dir/scan.o.requires:

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/scan.o.requires

spacemgr/CMakeFiles/spacemgr.dir/scan.o.provides: spacemgr/CMakeFiles/spacemgr.dir/scan.o.requires
	$(MAKE) -f spacemgr/CMakeFiles/spacemgr.dir/build.make spacemgr/CMakeFiles/spacemgr.dir/scan.o.provides.build
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/scan.o.provides

spacemgr/CMakeFiles/spacemgr.dir/scan.o.provides.build: spacemgr/CMakeFiles/spacemgr.dir/scan.o


# Object files for target spacemgr
spacemgr_OBJECTS = \
"CMakeFiles/spacemgr.dir/db.o" \
"CMakeFiles/spacemgr.dir/dirpage.o" \
"CMakeFiles/spacemgr.dir/heapfile.o" \
"CMakeFiles/spacemgr.dir/heappage.o" \
"CMakeFiles/spacemgr.dir/heaptest.o" \
"CMakeFiles/spacemgr.dir/page.o" \
"CMakeFiles/spacemgr.dir/scan.o"

# External object files for target spacemgr
spacemgr_EXTERNAL_OBJECTS =

spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/db.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/dirpage.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/heapfile.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/heappage.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/heaptest.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/page.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/scan.o
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/build.make
spacemgr/libspacemgr.a: spacemgr/CMakeFiles/spacemgr.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/charlottezhao/Desktop/Database/HeapPage-bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library libspacemgr.a"
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && $(CMAKE_COMMAND) -P CMakeFiles/spacemgr.dir/cmake_clean_target.cmake
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spacemgr.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
spacemgr/CMakeFiles/spacemgr.dir/build: spacemgr/libspacemgr.a

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/build

spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/db.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/dirpage.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/heapfile.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/heappage.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/heaptest.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/page.o.requires
spacemgr/CMakeFiles/spacemgr.dir/requires: spacemgr/CMakeFiles/spacemgr.dir/scan.o.requires

.PHONY : spacemgr/CMakeFiles/spacemgr.dir/requires

spacemgr/CMakeFiles/spacemgr.dir/clean:
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr && $(CMAKE_COMMAND) -P CMakeFiles/spacemgr.dir/cmake_clean.cmake
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/clean

spacemgr/CMakeFiles/spacemgr.dir/depend:
	cd /Users/charlottezhao/Desktop/Database/HeapPage-bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/charlottezhao/Desktop/Database/HeapPage /Users/charlottezhao/Desktop/Database/HeapPage/spacemgr /Users/charlottezhao/Desktop/Database/HeapPage-bin /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr /Users/charlottezhao/Desktop/Database/HeapPage-bin/spacemgr/CMakeFiles/spacemgr.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : spacemgr/CMakeFiles/spacemgr.dir/depend

