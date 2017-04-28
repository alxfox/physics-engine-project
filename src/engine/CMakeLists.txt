set( ENGINE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )

# Function to add sources in subdirectories
# Taken from https://cmake.org/pipermail/cmake/2010-March/035992.html
function( add_sources )
  get_property( isDefined GLOBAL PROPERTY ENGINE_SRC_LIST DEFINED )
  if( NOT isDefined )
    define_property( GLOBAL PROPERTY ENGINE_SRC_LIST
      BRIEF_DOCS "List of engine source files"
      FULL_DOCS "List of engine source files to be compiled in the static library" )
  endif( NOT isDefined )

  # make paths relative to parent directory
  file( RELATIVE_PATH relPath "${ENGINE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" )
  foreach( src ${ARGN} )
    if( relPath )
      list( APPEND srcs "${relPath}/${src}" )
    else( relPath )
      list( APPEND srcs "${src}" )
    endif( relPath )
  endforeach( src ${ARGN} )

  # append to global list
  set_property( GLOBAL APPEND PROPERTY ENGINE_SRC_LIST "${srcs}" )
endfunction( add_sources )

# Include all cpp files
add_sources( Collision.cpp
	CollisionResolver.cpp
	Engine.cpp )

add_subdirectory( constraints )
add_subdirectory( math )
add_subdirectory( messages )
add_subdirectory( objects )

# Create static library
get_property( ENGINE_SRC_FILES GLOBAL PROPERTY ENGINE_SRC_LIST )
add_library( engine STATIC ${ENGINE_SRC_FILES} )
