#include "Game.h"

#include <cmath>
#include <iostream>
#include "Scenarios.h"
#include "CustomScenarios.h"
#include "common/Entity.h"
#include "common/messages/StopMessage.h"
#include "engine/math/Quaternion.h"
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
  Entity m_front;
  Entity m_back;
  Entity m_right;
  Entity m_left;
  Entity m_roof;
  Entity m_floor;
  float time = 0;
  // Game loop
  int wave = 0;
  long int t0;
  long int t0_2;
  //std::cout << t0 << " seconds since the Epoch\n"<<std::endl;

	float width = 10.0f;
	float height = 30.0f;
	float depth = 100.0f;
	float arenaSize = 30.0f;
	float arenaHeight = 8.0f;
	float arenaOffset = -2.0f;
	float arenaThickness = 0.1f;

	gp::graphics::Material fullGreen; 
	gp::graphics::Material green; 
	gp::graphics::Material fullRed; 
	gp::graphics::Material red; 

  while (!glfwWindowShouldClose(m_window)) {
    if (m_scenarioControl.hasNewScenario()) {
      setupNewScenario();
      scenario = m_scenarioControl.scenario();
      spotLight = &scenario->spotLight();
      camera = &scenario->camera();
      renderComponentMan = &scenario->renderObjectManager();
      constraintMan = &scenario->renderConstraintManager();


      if (dynamic_cast<gp::Custom3*>(scenario) != nullptr){
        m_scenarioControl.m_score = 0;
        m_scenarioControl.m_life = 3;
        m_scenarioControl.m_lifeLabel->setCaption("LIVES: 3");
        m_scenarioControl.m_scoreLabel->setCaption("SCORE: 0");

        t0 = static_cast<long int> (std::time(NULL));
        t0_2 = static_cast<long int> (std::time(NULL));
	      fullGreen = scenario->getMaterial("fullGreen");
        fullGreen.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.0f);
      //=====================================================Our Game======================================================
      //(need to have references of some objects in Game.cpp, so as to make them interactive)
        //return;
	      gp::graphics::Material& white = scenario->getMaterial("white");

       	gp::graphics::Material& blue = scenario->getMaterial("blue");
       	blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);

	      green = scenario->getMaterial("green");
       	green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.2f);

	      red = scenario->getMaterial("red");
       	red.diffuseColor = engine::Vector3f(1.0f, 0.2f, 0.1f);

	      fullRed = scenario->getMaterial("fullRed");
        fullRed.diffuseColor = engine::Vector3f(1.0f, 0.0f, 0.0f);


	      m_front = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, 0.0, -depth/2.0), engine::Vector3f(width, height, arenaThickness),
        engine::Vector3f::Zero(), engine::Quaternion(), engine::Object::TRIGGER_ZONE_0);
	      scenario->setMaterial(m_front, green);
	      //scenario->setName(m_front, "front");

	      m_back = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0,  0.0, depth/2.0), engine::Vector3f(width, height, arenaThickness),
        engine::Vector3f::Zero(), engine::Quaternion(), engine::Object::TRIGGER_ZONE_1);
	      scenario->setMaterial(m_back, red);
	      //setName(e, "back");

	      m_left = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(-width/2.0, 0.0, 0.0), engine::Vector3f(arenaThickness, height, depth));
	      scenario->setMaterial(m_left, blue);
	      //setName(e, "left");

	      m_right = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(width/2.0, 0.0, 0.0), engine::Vector3f(arenaThickness, height, depth));
	      scenario->setMaterial(m_right, blue);
	      //setName(e, "right");

	      m_roof = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, height/2.0, 0.0), engine::Vector3f(width, arenaThickness, depth));
	      scenario->setMaterial(m_roof, white);
	      //setName(e, "right");

	      m_floor = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, -height/2.0, 0), engine::Vector3f(width, arenaThickness, depth));
	      scenario->setMaterial(m_floor, white);

        //Entity lamp;
	      //lamp = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, height/2.0-2, 0), engine::Vector3f(4.0f, 4.0f, 4.0f));
	      //scenario->setMaterial(lamp, white);
	      //setName(e, "top");

      }

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



    if (dynamic_cast<gp::Custom3*>(scenario) != nullptr){
    //=====================================================Our Game======================================================
      //(need to have references of some objects in Game.cpp, so as to make them interactive)
      //return;

      if(!m_scenarioControl.isPaused()){
        long int t1 = static_cast<long int> (std::time(NULL));
        if (t1 - t0_2 > 0.5f) {
          t0_2 = static_cast<long int> (std::time(NULL));
          scenario->setMaterial(m_front, green);
          scenario->setMaterial(m_back, red);
          wave+=1;
        }
        if (t1 - t0 > 0.5f) {
          t0 = static_cast<long int> (std::time(NULL));
          float rPos_x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * width - width/2.0f;
          float rPos_y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * height - height/2.0f;

          //from -depth/2 to depth/2 - 5
          float rPos_z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (depth-(depth-depth/9.0f)) - depth/2.0f;

          float rVel_x = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2.0f -1.0f)*
                          (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10.0f;
          float rVel_y = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2.0f -1.0f)*
                          (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10.0f;
          float rVel_z = ((static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*2.0f -1.0f)*
                          (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10.0f;

          float rVel_length = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10.0f;
          if ((rVel_length) > 5.0f){//very fast
              if(rPos_z > 0){//too close
                rPos_z-=depth/2.0f;
              }
          }
          float rRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.5f;
          scenario->addSphere(1.0f, engine::Vector3f(rPos_x, rPos_y, rPos_z), rRadius, 
                              rVel_length*engine::Vector3f(rVel_x, rVel_y, rVel_z+2.5f));
          wave+=1;
        }
      }
      else{
          //5 and 0.5 seconds after the pause, we will have the events, if there's no another pause in between.
          t0 = static_cast<long int> (std::time(NULL));
          t0_2 = static_cast<long int> (std::time(NULL));
      }

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

      if (gp::messages::isType<gp::engine::messages::EnemyDamageMessage>(message))
      {
        m_scenarioControl.m_life -= 1;
        m_scenarioControl.m_lifeLabel->setCaption("LIVES: "+ std::to_string(m_scenarioControl.m_life));
        scenario->setMaterial(m_back, fullRed);
      }
      if (gp::messages::isType<gp::engine::messages::EnemyDeathMessage>(message))
      {
        m_scenarioControl.m_score += 1;
        m_scenarioControl.m_scoreLabel->setCaption("SCORE:  "+ std::to_string(m_scenarioControl.m_score));
        scenario->setMaterial(m_front, fullGreen);
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
      break;
    }
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
