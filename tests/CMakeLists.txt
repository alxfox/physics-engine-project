# Load additional module path
set( CMAKE_MODULE_PATH
  "${CMAKE_SOURCE_DIR}/lib/cxxtest/build_tools/cmake"
  ${CMAKE_MODULE_PATH} )

# Find CxxTest
find_package( CxxTest REQUIRED )
include_directories( ${CXXTEST_INCLUDES} )

# Batch script does not seem to work on windows
if (WIN32)
  set(CXXTESTGEN python ${CXXTESTGEN})
endif()

# Base function to add tests
# (Modified copy from cxxtest)
function( cxx_test_base source )
  get_filename_component( CPP_FILE_NAME ${source} NAME )
  string( REGEX REPLACE "h$|hpp$" "cpp" CPP_FILE_NAME ${CPP_FILE_NAME} )
  set( CPP_FULL_NAME "${CMAKE_CURRENT_BINARY_DIR}/${CPP_FILE_NAME}" )
  string( REGEX REPLACE "(\\.t)?\\.cpp$" "" TEST_NAME ${CPP_FILE_NAME} )
  set( CXX_TEST_NAME ${TEST_NAME} PARENT_SCOPE )

  add_custom_command(
    OUTPUT "${CPP_FULL_NAME}"
    COMMAND ${CXXTESTGEN} --runner=ErrorPrinter --output "${CPP_FULL_NAME}" "${source}"
    DEPENDS "${source}"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )
  add_executable( ${TEST_NAME} ${CPP_FULL_NAME} )
if (NOT MSVC)
  set_target_properties( ${TEST_NAME} PROPERTIES COMPILE_FLAGS "-Wno-effc++" )
endif()

  add_test( ${TEST_NAME} ${RUNTIME_OUTPUT_DIRECTORY}/${TEST_NAME} )
endfunction( cxx_test_base )

# Function to add tests without library dependcies
function( cxx_test source )
  cxx_test_base( ${source} )
endfunction( cxx_test )

# Add tests
add_subdirectory( common )
add_subdirectory( engine )
