#include "Game.h"

#include <cmath>
#include <iostream>
#include "Scenarios.h"
#include "common/messages/StopMessage.h"
#include "engine/messages/ScenarioMessage.h"
#include "engine/objects/Box.h"
#include "engine/Engine.h"
#include "engine/utils.h"
#include "gui/graphics/RenderComponentManager.h"
#include "gui/graphics/Camera.h"
#include "gui/graphics/TextureManager.h"
#include "engine/messages/ControlMessage.h"
#include <GLFW/glfw3.h>
#include <ctime>
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
  float time = 0;
  // Game loop
  int wave = 0;
  std::time_t result = std::time(nullptr);
  long int t0 = static_cast<long int> (std::time(NULL));
  //std::cout << t0 << " seconds since the Epoch\n"<<std::endl;
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


    //if (glfwGetKey(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS&&!m_scenarioControl.interactsWithMouse()) {
	  if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS&&!m_scenarioControl.interactsWithMouse()) {
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

      m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 8;
      gp::engine::Vector4f shootColor(255.0f, 0.0f, .0f, 955.0f);
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(shootColor);
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(shootColor);
		}
    else{
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 4;
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));

    }

    glm::mat4 m = camera->view;
    glm::vec3 lookAt(m[0][2],m[1][2],m[2][2]);
    gp::engine::Vector3f towards(-lookAt.x,-lookAt.y,-lookAt.z);
    towards.normalize();
    glm::vec3 pos = camera->worldPosition();
    gp::engine::Vector3f camPos(pos.x,pos.y,pos.z);
		m_vis2engine.push(gp::engine::messages::PlayerPositionMessage(camPos - towards*0.4f));



    long int t1 = static_cast<long int> (std::time(NULL));
    if (t1 - t0 > 2) {
      t0 = static_cast<long int> (std::time(NULL));
      scenario->addSphere(1.0f, engine::Vector3f(0.75f, 2.0f + wave, -0.9f), 0.7f);
      wave+=1;
    }
    //if (frameCounter % 6000 == 0) { 

    //  scenario->addSphere(1.0f, engine::Vector3f(0.75f, 2.0f, -0.9f), 0.7f);
    //  std::cout << "{" 
    //    << scenario->camera().worldPosition().x << " " << scenario->camera().worldPosition().y << " " << scenario->camera().worldPosition().z 
    //    << "}" << std::endl; 
    //}
     //std::cout << "sice " << m_engine2vis.size() << std::endl;
    if(!m_engine2vis.empty()) {
      gp::messages::Message message = m_engine2vis.pop();

      if (gp::messages::isType<gp::engine::messages::ScoreAndLifeUpdate>(message))
      {
        const gp::engine::messages::ScoreAndLifeUpdate& me = static_cast<const gp::engine::messages::ScoreAndLifeUpdate&>(message);
        //std::cout << "vaaaaaaaaca" << std::endl;
        //auto castedMsg = static_cast<gp::engine::messages::ScoreAndLifeUpdate&>(message);
        //const gp::engine::messages::ScoreAndLifeUpdate* me = dynamic_cast<gp::engine::messages::ScoreAndLifeUpdate*>(message);
		    //std::cout << "hehehehe" << me.get_life() << std::endl;
        float_t life = me.get_life();
        float_t score = me.get_score();
        //update the window 
        m_scenarioControl.m_leben->setCaption("LIVES:"+ std::to_string(life));
        m_scenarioControl.m_punkte->setCaption("SCORE:"+ std::to_string(score));
      }
    }

    scenario->synchronize();

    renderComponentMan->computeMatrices(camera->view, spotLight->depthVP);
    renderComponentMan->sortForRender();
    m_render.render(*renderComponentMan, *constraintMan, *camera, *spotLight, m_textureMan, m_scenarioControl.enableConstraintsRendering);

    m_scenarioControl.drawWidgets();

    char buffer[512];
    if (frameCounter % 60 == 0) {
      float secs = m_render.lastGPUTime();
      time += secs;
      sprintf(buffer, "Game physics lab. Render %.2lf ms. %.2lf", secs* 1.0e-6, time*1.0e-6);
      glfwSetWindowTitle(m_window, buffer);

    }
    ++frameCounter;

    glfwSwapBuffers(m_window);

    // if (!m_engine2vis.empty()) {
    //   dealWithMsgFromEngine();
    // }
  }
}

void gp::Game::setupNewScenario()
{
	m_vis2engine.push(gp::engine::messages::ScenarioMessage(
		m_scenarioControl.peekScenario()->engineObjectManager(),
		m_scenarioControl.peekScenario()->engineConstraintManager()));
	/// \todo general message handler+
  while (true){
  gp::messages::Message message = m_engine2vis.waitAndPop();
    if(gp::messages::isType<gp::engine::messages::ScenarioLoadedMessage>(message)){
      const gp::engine::messages::ScenarioLoadedMessage& me = static_cast<const gp::engine::messages::ScenarioLoadedMessage&>(message);
      std::cout << "yeeeha " << me.m_test << std::endl; 
      break;
    }
    //else if (gp::messages::isType<gp::engine::messages::ScoreAndLifeUpdate>(message))
    //{
    //  std::cout << "vaaaaaaaaca" << std::endl;
    //  auto castedMsg = static_cast<gp::engine::messages::ScoreAndLifeUpdate&>(message);
    //uint16_t life = 2318; //castedMsg.get_life();
    //uint16_t score = castedMsg.get_score();
    ////update the window 
    //m_scenarioControl.m_leben->setCaption("LIVES:  	 "+ std::to_string(life));
    ////m_scenarioControl.m_punkte = new Label(points, "SCORE: 	"+ std::to_string(score));

    //}
  }
	

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
