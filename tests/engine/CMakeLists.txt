#Link cxx tests with engine
function( cxx_test source )
  cxx_test_base( ${source} )
  target_link_libraries( ${CXX_TEST_NAME} engine -pthread)
endfunction( cxx_test )

cxx_test( ObjectManager.t.h )
cxx_test( Collision.t.h )

add_subdirectory( math )
add_subdirectory( objects )