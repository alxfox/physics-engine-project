# Link cxx tests with pthreads
function( cxx_test source )
  cxx_test_base( ${source} )
  target_link_libraries( ${CXX_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT} )
endfunction( cxx_test )

cxx_test( Queue.t.h )
set_tests_properties( Queue PROPERTIES TIMEOUT 10 )