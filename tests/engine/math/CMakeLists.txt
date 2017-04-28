# (Currently) No linking required for math tests
function( cxx_test source )
  cxx_test_base( ${source} )
  target_link_libraries( ${CXX_TEST_NAME} )
endfunction( cxx_test )

cxx_test( AABox.t.h )
cxx_test( BoxProjection.t.h )
cxx_test( Line.t.h )
cxx_test( Quaternion.t.h )
cxx_test( SkewSymmetricMatrix.t.h )