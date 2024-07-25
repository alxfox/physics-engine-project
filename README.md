# Game Physics Project
Implementation of a game physics engine in C++ for the Game Physics course at the Technical University Munich.

As part of this project, collision detection and handling was implemented to enable an accurate physics simulation.\
Supported objects include spheres, cuboids, and tethers, the latter of which may be used to physically tie other objects to each other.

![Physics Scene](/assets/physics_screenshot.png?raw=true "Physics Scene")

For the final part of the project, a simple shooting game was conceptualized and implemented.\
Notable parts of this implementation include support for cursor-based interaction with the 3D environment, as well as support for collision-based triggers to facilitate a scoring system.

![Game Scene](/assets/game_screenshot.png?raw=true "Game Scene")


## General Information:

### Enviroment
The officially supported OS for this cource is Ubuntu (Linux) between versions 14.04 and 20.04. We strongly recomend you to use VirtualBox in case if your host OS is either Windows or MacOS. It doesn't mean that our engine cannot get compiled for other OS's. It is done on purpose to provide a better support to students in case of any problems with compiling and linking.

### Installation Process

#### Prerequisites
```console
$ sudo apt-get update
$ sudo apt-get install gcc-<version> g++-<version>
$ sudo apt-get install python
$ sudo apt-get install cmake
$ sudo apt-get install libglu1-mesa-dev freeglut3-dev \
mesa-common-dev libgl1-mesa-dev xorg-dev
$ sudo apt-get install doxygen
```

Where \<version\> can be 5, 6 or 8 (**but not 7**)

#### Compiling
```console
$ git clone https://gitlab.lrz.de/tum-i5/GamePhysics-public.git
$ cd GamePhysics-public
$ mkdir build && cd build
$ cmake .. -DCMAKE_CXX_COMPILER=$(which g++-<version>) \
-DCMAKE_C_COMPILER=$(which gcc-<version>)
$ make -j4
```

#### Running
Make sure that everything works correctly by running the engine once you've compiled the code:
```console
$ ./gamephysics
``` 

#### Docs
It is quite useful to look at the documentation from time to time. The documentation also shows caller and callee UML diagrams apart from describing methods and members of classes. You can find documentation in your **build** directory.
```console
$ cd doc
$ doxygen Doxyfile
$ firefox html/index.html
```
