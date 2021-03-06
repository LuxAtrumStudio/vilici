cmake_minimum_required(VERSION 3.8.2) # setting this is required
project(Vilici VERSION 0.0.0 LANGUAGES CXX) # this sets the project name

set(default_build_type "Release")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
  set(default_build_type "Debug")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(
    STATUS
      "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE
      "${default_build_type}"
      CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")
endif()
if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json")
  execute_process(COMMAND ${CMAKE_COMMAND}
                          -E
                          copy_if_different
                          ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json
                          ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy") set(CMAKE_CXX_CPPCHECK "cppcheck")
# set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE "iwyu-tool")

string(TOUPPER ${CMAKE_BUILD_TYPE} CMAKE_BUILD_TYPE_DEFINE)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -D${CMAKE_BUILD_TYPE_DEFINE})

# ##############################################################################
# file globbing ##############################################################
# ##############################################################################

# These instructions search the directory tree when cmake is invoked and put all
# files that match the pattern in the variables `sources` and `data`.
file(GLOB_RECURSE
     sources
     src/*.cpp
     src/*.h)
file(GLOB_RECURSE data resources/*)
# You can use set(sources src/main.cpp) etc if you don't want to use globbing to
# find files automatically.

# ##############################################################################
# target definitions #########################################################
# ##############################################################################

# The data is just added to the executable, because in some IDEs (QtCreator)
# files are invisible when they are not explicitly part of the project.
add_executable(vilici ${sources} ${data})

# Just for example add some compiler flags.
target_compile_options(vilici PUBLIC -Wall -Wfloat-conversion)
set_target_properties(vilici
                      PROPERTIES CXX_STANDARD
                                 17
                                 CXX_STANDARD_REQUIRED
                                 YES
                                 CXX_EXTENSIONS
                                 NO)

# This allows to include files relative to the root of the src directory with a
# <> pair
target_include_directories(vilici PUBLIC src/)

# This copies all resource files in the build directory. We need this, because
# we want to work with paths relative to the executable.
file(COPY ${data} DESTINATION resources)

# ##############################################################################
# dependencies ###############################################################
# ##############################################################################

include(ExternalProject)
find_package(Threads REQUIRED)
find_package(Fontconfig REQUIRED)
find_package(Libinput REQUIRED)
find_package(udev REQUIRED)
find_package(Libevdev REQUIRED)
add_subdirectory(${PROJECT_SOURCE_DIR}/external/freetype2)
target_include_directories(
  vilici
  PRIVATE ${PROJECT_SOURCE_DIR}/external/freetype2/include
          ${Fontconfig_INCLUDE_DIRS}
          ${Libinput_INCLUDE_DIRS}
          ${UDEV_INCLUDE_DIRS}
          ${LIBEVDEV_INCLUDE_DIRS})
target_link_libraries(vilici
                      ${CMAKE_THREAD_LIBS_INIT}
                      freetype
                      ${Fontconfig_LIBRARIES}
                      ${Libinput_LIBRARIES}
                      ${UDEV_LIBRARIES}
                      ${LIBEVDEV_LIBRARIES})

# ##############################################################################
# packaging ##################################################################
# ##############################################################################

# All install commands get the same destination. this allows us to use paths
# relative to the executable.
install(TARGETS vilici DESTINATION vilici_destination)

# This is basically a repeat of the file copy instruction that copies the
# resources in the build directory, but here we tell cmake that we want it in
# the package.
install(DIRECTORY resources DESTINATION vilici_destination)

# Now comes everything we need, to create a package there are a lot more
# variables you can set, and some you need to set for some package types, but we
# want to be minimal here.
set(CPACK_PACKAGE_NAME "Vilici")
set(
  CPACK_PACKAGE_VERSION
  "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}")

# We don't want to split our program up into several incomplete pieces.
set(CPACK_MONOLITHIC_INSTALL TRUE)

# This must be last
include(CPack)
