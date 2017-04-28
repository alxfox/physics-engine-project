/**
 * @mainpage
 *
 * GamePhysics is a simple momentum-based physics engine for 3D games used in a lab course at <a href="https://www.tum.de">Technical University of Munich</a>.
 * The engine is able to detect the collision of simple objects (spheres and boxes) and resolve the detected collisions.
 * Collision are resolved by moving objects and applying correct linear (and angular) momentum to the objects.
 * Many parts of the engine are designed after the book "Game Physics Engine Development" by Ian Millington.
 * The full code of physics engine is not available publicly since it is the task of the lab course to implement the missing parts.
 * GamePhysics cames with a <a href="https://www.opengl.org">OpenGL</a> visualization that can be used for debugging, testing and to implement small games.
 *
 * GamePhysics is written in C++11 and is based on several other libraries and tools:
 *  - <a href="https://cmake.org/">CMake</a>: A cross-platform build tool
 *  - <a href="https://github.com/wjakob/nanogui">NanoGUI</a>: A small but powerful corss-platform widget library for OpenGL
 *  - <a href="http://eigen.tuxfamily.org/">Eigen</a>: A C++ template library for linear algebra
 *  - <a href="http://glm.g-truc.net/0.9.8/index.html">GLM</a>: A C++ template library based on the OpenGL Shading Language (GLSL)
 *  - <a href="https://github.com/nothings/stb">stb_image</a>: Image loading/decoding library
 *  - <a href="http://cxxtest.com/">CxxTest</a>: A testing framework for C++
 *
 * Further topics:
 *  - @ref overview
 *  - @ref compilation
 *  - @ref debugging
 */