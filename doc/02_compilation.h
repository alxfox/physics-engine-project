/**
 * @page compilation Compilation
 *
 * Compiling GamePhysics is done with CMake.
 *
 * On Linux, CMake is used to generate Makefiles. Assuming that you are in the root directory of the project, the following commands can be used to build the code:
 * @code{.sh}
 * mkdir build
 * cd build/
 * cmake ../
 * make
 * @endcode
 *
 * After successfully building the code, start the application with
 * @code{.sh}
 * ./gamephysics
 * @endcode
 *
 * Once you have build the project, you can also run the tests using make:
 * @code{.sh}
 * make test
 * @endcode
 *
 * To generate this documentation (requireds <a href="https://www.stack.nl/~dimitri/doxygen/">Doxygen</a>, run:
 * @code{.sh}
 * make doc
 * @endcode
 */