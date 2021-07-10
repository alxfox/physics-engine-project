#include "Game.h"

#include <iostream>
#include "Scenarios.h"
#include "common/messages/StopMessage.h"
#include "engine/messages/ScenarioMessage.h"
#include "engine/objects/Box.h"
#include "engine/Engine.h"
#include "gui/graphics/RenderComponentManager.h"
#include "gui/graphics/Camera.h"
#include "gui/graphics/TextureManager.h"
#include "engine/messages/ControlMessage.h"

void* gp::runEngine(void* data)
{
	EngineData* engineData = static_cast<EngineData*>(data);

	gp::engine::Engine engine(engineData->vis2engine, engineData->engine2vis);
	engine.start();

	return 0L;
}

gp::Game::Game(GLFWwindow* window)
  : m_window(window), m_scenarioControl(window, m_vis2engine)
{
	m_scenarioControl.loadScenario<gp::Scenario1>();

	// Start the engine thread
	EngineData engineData = {m_vis2engine, m_engine2vis};
	pthread_create(&m_engineThread, 0L, runEngine, &engineData);

	m_textureMan.addDirectory("../textures/");
}

gp::Game::~Game()
{
	// Send stop message to game engine thread
	m_vis2engine.push(gp::messages::StopMessage());

	// Wait for the engine thread to finish
	pthread_join(m_engineThread, 0L);
}

void gp::Game::run()
{
  gp::Scenario* scenario = nullptr;
  gp::graphics::SpotLight* spotLight = nullptr;
  gp::graphics::Camera* camera = nullptr;
  gp::graphics::RenderComponentManager* renderComponentMan = nullptr;
  gp::graphics::ConstraintManager* constraintMan = nullptr;

  GLuint64 frameCounter = 0;
  // Game loop
  while (!glfwWindowShouldClose(m_window)) {
    if (m_scenarioControl.hasNewScenario()) {
      setupNewScenario();
      scenario = m_scenarioControl.scenario();
      spotLight = &scenario->spotLight();
      camera = &scenario->camera();
      renderComponentMan = &scenario->renderObjectManager();
      constraintMan = &scenario->renderConstraintManager();
    }
    // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    m_cameraControl.moveCamera(m_window, m_scenarioControl, *camera);
    if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS&&!m_scenarioControl.interactsWithMouse()) {
      glm::vec3 pos = camera->worldPosition();
      gp::engine::Vector3f posV(pos.x,pos.y,pos.z);
      /**This also works**/
      //glm::quat q = glm::inverse(glm::normalize(camera->rotation()));
      //glm::vec3 lookAt = q*glm::vec3(0,0,1);
      glm::mat4 m = camera->view;
      glm::vec3 lookAt(m[0][2],m[1][2],m[2][2]);
      gp::engine::Vector3f towards(-lookAt.x,-lookAt.y,-lookAt.z);
      towards.normalize();
      //scenario->addSphere(gp::engine::Object::UNMOVABLE_MASS,posV + towards, 0.01f);
			m_vis2engine.push(gp::engine::messages::ShootMessage(posV,towards));
		}

    if (frameCounter % 2 == 0) {
      glm::quat q = camera->rotation();
      glm::vec3 lookAt = q*glm::vec3(0,0,1);
      gp::engine::Vector3f towards(lookAt.x,lookAt.y,-lookAt.z);//why do i need the -?
      glm::vec3 pos = camera->worldPosition();
      gp::engine::Vector3f camPos(pos.x,pos.y,pos.z);
			m_vis2engine.push(gp::engine::messages::PlayerPositionMessage(camPos - towards*0.15f));
    }
    if (frameCounter % 600 == 0) { 

      scenario->addSphere(1.0f, engine::Vector3f(0.75f, 2.0f, -0.9f), 0.7f);
      std::cout << "{" 
        << scenario->camera().worldPosition().x << " " << scenario->camera().worldPosition().y << " " << scenario->camera().worldPosition().z 
        << "}" << std::endl; 
    }
    scenario->synchronize();

    renderComponentMan->computeMatrices(camera->view, spotLight->depthVP);
    renderComponentMan->sortForRender();
    m_render.render(*renderComponentMan, *constraintMan, *camera, *spotLight, m_textureMan, m_scenarioControl.enableConstraintsRendering);

    m_scenarioControl.drawWidgets();

    char buffer[512];
    if (frameCounter % 60 == 0) {
      sprintf(buffer, "Game physics lab. Render %.2lf ms.", m_render.lastGPUTime() * 1.0e-6);
      glfwSetWindowTitle(m_window, buffer);

    }
    ++frameCounter;

    glfwSwapBuffers(m_window);
  }
}

void gp::Game::setupNewScenario()
{
	m_vis2engine.push(gp::engine::messages::ScenarioMessage(
		m_scenarioControl.peekScenario()->engineObjectManager(),
		m_scenarioControl.peekScenario()->engineConstraintManager()));
	/// \todo general message handler
	m_engine2vis.waitAndPop();

  m_scenarioControl.swapScenario();
  m_cameraControl.reset();

  int width, height;
  glfwGetFramebufferSize(m_window, &width, &height);
  m_scenarioControl.scenario()->camera().resize(width, height);

  setupCallbacks();
}

void gp::Game::setupCallbacks()
{
  wp.camera = &m_scenarioControl.scenario()->camera();
  wp.screen = &m_scenarioControl;

  glfwSetWindowUserPointer(m_window, &wp);

  glfwSetWindowSizeCallback(m_window,
    [](GLFWwindow* window, int width, int height) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->camera->resize(width, height);
  });

  glfwSetCursorPosCallback(m_window,
    [](GLFWwindow* window, double x, double y) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->cursorPosCallbackEvent(x, y);
  });

  glfwSetMouseButtonCallback(m_window,
    [](GLFWwindow* window, int button, int action, int modifiers) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->mouseButtonCallbackEvent(button, action, modifiers);
  });

  glfwSetKeyCallback(m_window,
    [](GLFWwindow* window, int key, int scancode, int action, int mods) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->keyCallbackEvent(key, scancode, action, mods);
  });

  glfwSetCharCallback(m_window,
    [](GLFWwindow* window, unsigned int codepoint) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->charCallbackEvent(codepoint);
  });

  glfwSetDropCallback(m_window,
    [](GLFWwindow* window, int count, const char **filenames) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->dropCallbackEvent(count, filenames);
  });

  glfwSetScrollCallback(m_window,
    [](GLFWwindow* window, double x, double y) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->scrollCallbackEvent(x, y);
  });

  glfwSetFramebufferSizeCallback(m_window,
    [](GLFWwindow* window, int width, int height) {
      static_cast<WindowPointer*>(glfwGetWindowUserPointer(window))->screen->resizeCallbackEvent(width, height);
  });
}
