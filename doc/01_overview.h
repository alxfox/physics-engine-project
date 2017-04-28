/**
 * @page overview An Overview of the Code
 *
 * @section physics_visualization_engine The Physics and Visualization Engine
 *
 * GamePhysics is split into two parts, the physics engine and the visualization engine.
 * The division is reflected in the code and the execution model.
 * The physics engine code is in <code>src/engine</code> while the window management and visualization engine in <code>src/gui</code>.
 * The directory <code>src/common</code> contains code that is no directly related to the physics and visualization engine and some of these classes are used by both engines.
 * For the exercises of the lab course it is not required to understand the code in <code>src/gui</code>.
 * The main focus will be on the physics engine in <code>src/engine</code>.
 * However, it might be nesessary to interact with some of the classes in <code>src/common</code>, e.g. the {@link gp::VectorComponentManager}.
 *
 * During runtime, the physics and visualization engine utilize two different threads to make progress independent of each other.
 * Thus, a slow graphics card does not prevent you from an accurate physics simulation and a slow CPU does not necessarily result in an unusable GUI.
 * Note that the main program thread is used for the GUI and visualization engine.
 * The physics engine thread is controlled by the class {@link gp::Game} and automatically started when the constructor is called.
 *
 * @subsection communication Communication between physics and visualization engine
 *
 * The physics and visualization can communicate via message {@link gp::messages::Queue}s.
 * A {@link gp::messages::Queue} is a partly thread-safe one-way communication channel.
 * It is only partly thread-safe because it is not possible call {@link gp::messages::Queue::push} from multiple threads at the same time.
 * In the same way, {@link gp::messages::Queue::pop} can only be used from one thread.
 * However, {@link gp::messages::Queue::push} and {@link gp::messages::Queue::pop} can be used from two threads at the same time, i.e. one thread can send messages while another receives them asynchronously.
 * With {@link gp::messages::Queue}s a bi-directional message system is established.
 *
 * Since the visualization engine needs the updated positions and rotations (i.e. the model matrix) of all objects after each time step, a second read-only communication is used between the two engines.
 * After each frame, the visualization engine reads the model matrix from the physics engine and stores it in its own data structure.
 * This is done by calling {@link gp::Scenario::synchronize()} on the active scenario.
 *
 * @subsection entities The entity system
 *
 * To trac objects between the to engines, a entity system is used.
 * The entity systems consists of an {@link gp::EntityManager}, {@link gp::Entity}s and several {@link gp::ComponentManager}s.
 * The {@link gp::EntityManager} is used to create new {@link gp::Entity}s.
 * An {@link gp::Entity} is only and identifier for an object.
 * It has no useful attributes or functions.
 * However, you can add components to an entity through a {@link gp::ComponentManager}.
 * How the components look like and how they are stored is completely up the {@link gp::ComponentManager}.
 *
 * It is possible to create multiple components for a single {@link gp::Entity} using different component managers.
 * In the code, this done for {@link gp::engine::Object}s and {@link gp::engine::HardConstraint}s.
 * Each {@link gp::engine::Object} and {@link gp::engine::HardConstraint} has two components, one storing the physics information (e.g. velocity) and one storing visualization information (e.g. color).
 * Some informations are stored in both components (e.g. position) and need to be synchronized manually (see Section @ref communication).
 *
 * The entity system is important for creating new {@link gp::Scenario}s.
 * For all normal {@link gp::engine::Object}s (and {@link gp::engine::HardConstraint}s) you need to create a physics and visualization component.
 * In the physics engine the {@link gp::Entity}s are not required since from the {@link gp::engine::ObjectManager} and the {@link gp::engine::ConstraintManager}, the {@link gp::engine::Object}s and {@link gp::engine::HardConstraint}s are directly accessible through {@link gp::VectorComponentManager::objects()}.
 *
 * @subsection physics_engine The physics engine
 *
 * The main implementation of the physics engine can be found in {@link gp::engine::Engine}.
 * The entry point is {@link gp::engine::Engine::start} which runs the physics engine in an "infinite loop".
 *
 * The main steps of the loop are:
 *  - Handling incoming message
 *  - Advance to the next time step
 *  - Detect collision
 *  - Resolve interpenetrations
 *  - Apply a collision impulse
 *
 * Since the physics engine can only handle collisions between two objects and resolving one collision can result in another collision, collisions are resolved several times in one time step until (hopefully) all collisions are resolved.
 *
 * After each time step, the physics engine sleeps until the time configured in {@link gp::engine::UPDATE_INTERVAL} has passed.
 * For example, if the collision detection and resolution took 5 ms and {@link gp::engine::UPDATE_INTERVAL} is 10 ms, the engine will sleep for another 5 ms before the next time step is executed.
 * This approach assume that the collision detection and resolution takes less than the time configured in {@link gp::engine::UPDATE_INTERVAL}.
 * If you have to many objects (or your CPU is to slow), the physics engine will run in real-time.
 */