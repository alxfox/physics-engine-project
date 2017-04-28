cmake_minimum_required( VERSION 2.8.12 )
project( GamePhysics )

set( GamePhysics_VERSION_MAJOR 0 )
set( GamePhysics_VERSION_MINOR 1 )

# Options
option( UNIT_TESTS "build unit tests" ON )

if (MSVC)
	set(NANOGUI_SHARED OFF)
else()
	set(NANOGUI_SHARED ON)
endif()

# Add nanogui
include( ExternalProject )
ExternalProject_Add( nanogui-lib
  PREFIX ${CMAKE_BINARY_DIR}/lib
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib/nanogui
  CMAKE_ARGS -DNANOGUI_BUILD_EXAMPLE=OFF -DNANOGUI_BUILD_PYTHON=OFF -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib -DNANOGUI_USE_GLAD=ON -DNANOGUI_BUILD_SHARED=${NANOGUI_SHARED} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
include_directories( ${CMAKE_BINARY_DIR}/lib/include )
link_directories( ${CMAKE_BINARY_DIR}/lib/lib )
# Add eigen
include_directories( ${CMAKE_SOURCE_DIR}/lib/nanogui/ext/eigen )
# Add GLFW
include_directories( ${CMAKE_SOURCE_DIR}/lib/nanogui/ext/glfw/include )
# Add nanovg
include_directories( ${CMAKE_SOURCE_DIR}/lib/nanogui/ext/nanovg/src )
# Add glad
include_directories( ${CMAKE_SOURCE_DIR}/lib/nanogui/ext/glad/include )
# Add glm
include_directories( ${CMAKE_SOURCE_DIR}/lib/glm )

# Add pthreads
find_package( Threads REQUIRED )
find_package( OpenGL REQUIRED )

# Function to add sources in subdirectories
# Taken from https://cmake.org/pipermail/cmake/2010-March/035992.html
function( add_sources )
  get_property( isDefined GLOBAL PROPERTY SRC_LIST DEFINED )
  if( NOT isDefined )
    define_property( GLOBAL PROPERTY SRC_LIST
      BRIEF_DOCS "List of source files"
      FULL_DOCS "List of source files to be compiled in the executable" )
  endif( NOT isDefined )

  # make paths relative to parent directory
  message(${CMAKE_SOURCE_DIR})
  message(${CMAKE_CURRENT_SOURCE_DIR})
  file( RELATIVE_PATH relPath "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )
  foreach( src ${ARGN} )
    if( relPath )
      list( APPEND srcs "${relPath}/${src}" )
    else( relPath )
      list( APPEND srcs "${src}" )
    endif( relPath )
  endforeach( src ${ARGN} )

  # append to global list
  set_property( GLOBAL APPEND PROPERTY SRC_LIST "${srcs}" )
endfunction( add_sources )

# Set C++11
if (NOT MSVC)
  add_compile_options( -std=c++11 -DNANOGUI_GLAD )
endif()

# Enable warnings
if( CMAKE_COMPILER_IS_GNUCC )
  add_compile_options( -Wall -Wextra )
endif( CMAKE_COMPILER_IS_GNUCC )
if( MSVC )
  add_compile_options( /W3 )
  add_compile_options( /DHAVE_STRUCT_TIMESPEC /DNANOGUI_GLAD /DGLM_FORCE_CXX98 /D_CRT_SECURE_NO_WARNINGS )
  include_directories( ${CMAKE_SOURCE_DIR}/lib/pthreads-win32/include )
  set(CMAKE_THREAD_LIBS_INIT ${CMAKE_SOURCE_DIR}/lib/pthreads-win32/lib/pthreadVC2.lib)
endif()

# Include directories
include_directories( src )

# Parse subdirectories
add_subdirectory( src )

# Generate documentation
add_subdirectory( doc )

# Build executable
get_property( SRC_FILES GLOBAL PROPERTY SRC_LIST )
add_executable( gamephysics ${SRC_FILES} )
add_dependencies( gamephysics nanogui-lib )
target_link_libraries( gamephysics engine nanogui ${OPENGL_LIBS} ${CMAKE_THREAD_LIBS_INIT} )

if (MSVC)
  add_custom_command(TARGET gamephysics PRE_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy
                       ${CMAKE_SOURCE_DIR}/lib/pthreads-win32/dll/x64/pthreadVC2.dll $<TARGET_FILE_DIR:gamephysics>)
endif()

# Generate unit tests
if( ${UNIT_TESTS} )
  enable_testing()
  add_subdirectory( tests )
endif( ${UNIT_TESTS} )
