# No linking required for object tests
function( cxx_test source )
  cxx_test_base( ${source} )
  target_link_libraries( ${CXX_TEST_NAME} )
endfunction( cxx_test )

cxx_test( Object.t.h )