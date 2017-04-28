/**
 * @page debugging Debugging
 *
 * @section testing Testing
 *
 * The best way to debug the code is to write small tests that cover all (or at least most) of the situations in the code.
 * The project include <a href="http://cxxtest.com/">CxxTest</a>, a testing framework for C++.
 * In this project, the framework is integrated into CMake to simplify testing.
 * Simply execute <code>make test</code> after building the project to run all tests.
 * Alternatively, you can use <code>ctest</code> to run only selected tests.
 * The project comes with a view example tests for selected classes.
 * We encourage you to write additional tests for the physics engine to cover as many errors as possible in your the early development.
 *
 * @section printf Printf
 *
 * Adding print statements to the code can often provide helpful information without using complicated debuggers.
 * The matrix and vector datatypes of Eigen can be used together with C++ IO streams:
 * @code{.cpp}
 * Vector3f vec;
 * std::cout << vec << std::endl;
 * @endcode
 *
 * Assign names to objects in the scenario, to trac which object you are currently dealing with.
 * In addition, use the pause and step function of the GUI to manually step through the time steps.
 *
 * @section debuggers Debuggers
 *
 * On Linux, GDB is a very powerful debugger and most IDEs come with a graphical user interface for GDB.
 * When using a debugger, recall that the engine uses multiple threads.
 *
 * If you do not have a graphical interface for GDB, you can also use it from the command line (e.g. to find segmentation faults).
 * Start the code with
 * @code{.sh}
 * gdb ./gamephysics
 * @endcode
 * Start the program with:
 * @code
 * run
 * @endcode
 * Once the code crashed, get the call stack with:
 * @code
 * thread apply all backtrace
 * @endcode
 *
 * It is even possible to set break points, access variables and step through the program using the command line but not really convenient.
 *
 * When using a debugger, remember to change the build type in CMake to include debug information in your executable.
 */