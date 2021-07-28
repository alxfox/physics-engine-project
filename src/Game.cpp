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
#include <chrono>
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
  m_scenarioControl.toggleEngine();
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
  long int t0_ball;
  long int t0_squares;

  int numObjectsToDespawn = 10;
  int numObjectsToSpawn = 10;
  int level = 0;
  float_t levelVel = 1;
  int lives = 5;
  int score = 0;
  Entity spawnedObjects[30];
  int spawnIndex = 0;
  bool newLevel = false;
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
  gp::graphics::Material purple;
  gp::graphics::Material lightBlue;
  gp::graphics::Material darkBlue;
  gp::graphics::Material lightRed;
  gp::graphics::Material blue;
  gp::graphics::Material white;
  gp::graphics::Material pistacio;
	gp::graphics::Material fullOrange; 
	gp::graphics::Material orange; 
	gp::graphics::Material lightOrange; 


  bool currentlyChargedShooting =false;
  bool currentlyShooting = false;
  std::chrono::steady_clock::time_point startShot;
  std::chrono::steady_clock::time_point lastShot;
  bool canShoot = true;
  int32_t shotIntensityLevel = 0;
  int32_t cooldown = 1000;
  while (!glfwWindowShouldClose(m_window)) {
    if (m_scenarioControl.hasNewScenario()) {
      setupNewScenario();
      scenario = m_scenarioControl.scenario();
      spotLight = &scenario->spotLight();
      camera = &scenario->camera();
      renderComponentMan = &scenario->renderObjectManager();
      constraintMan = &scenario->renderConstraintManager();
      //m_cameraControl.reset();

      if(m_scenarioControl.m_reloadedScenario){
            numObjectsToDespawn = 10;
            numObjectsToSpawn = 10;
            level = 0;
            levelVel = 1;
            lives = 5;
            score = 0;
            m_scenarioControl.m_reloadedScenario = false;
            m_scenarioControl.m_lifeLabel->setCaption("LIVES: "+ std::to_string(lives));
            m_scenarioControl.m_scoreLabel->setCaption("SCORE: "+ std::to_string(score));
            m_scenarioControl.m_moobsLabel->setCaption("ENEMIES: "+ std::to_string(numObjectsToDespawn));
            m_scenarioControl.m_levelLabel->setCaption("LEVEL: "+ std::to_string(level+1));
            spawnIndex = 0;
      }



      if (dynamic_cast<gp::Custom3*>(scenario) != nullptr){

        t0 = static_cast<long int> (std::time(NULL));
        t0_ball = static_cast<long int> (std::time(NULL));
        t0_squares = static_cast<long int> (std::time(NULL));

      //=====================================================Our Game======================================================
      //(need to have references of some objects in Game.cpp, so as to make them interactive)
        //return;
	      white = scenario->getMaterial("white");

       	lightBlue = scenario->getMaterial("lightBlue");
       	lightBlue.diffuseColor = engine::Vector3f(0.01f, 0.53f, 0.82f);

       	blue = scenario->getMaterial("blue");
       	blue.diffuseColor = engine::Vector3f(0.0f, 0.5f, 1.0f);

       	darkBlue = scenario->getMaterial("darkBlue");
       	darkBlue.diffuseColor = engine::Vector3f(0.2f, 0.25f, 0.6f);

       	purple = scenario->getMaterial("purple");
       	purple.diffuseColor = engine::Vector3f(0.3f, 0.2f, 0.66f);

	      pistacio = scenario->getMaterial("pistacio");
       	pistacio.diffuseColor = engine::Vector3f(0.5f, 0.6f, 0.1f);

	      green = scenario->getMaterial("green");
       	green.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.2f);

	      fullGreen = scenario->getMaterial("fullGreen");
        fullGreen.diffuseColor = engine::Vector3f(0.0f, 1.0f, 0.0f);

	      lightRed= scenario->getMaterial("lightRed");
       	lightRed.diffuseColor = engine::Vector3f(1.0f, 0.5f, 0.5f);

	      red = scenario->getMaterial("red");
       	red.diffuseColor = engine::Vector3f(1.0f, 0.09f, 0.3f);

	      fullRed = scenario->getMaterial("fullRed");
        fullRed.diffuseColor = engine::Vector3f(0.8f, 0.2f, 0.2f);

	      lightOrange = scenario->getMaterial("lightOrange");
       	lightOrange.diffuseColor = engine::Vector3f(1.0f, 0.8f, 0.5f);

	      orange = scenario->getMaterial("orange");
       	orange.diffuseColor = engine::Vector3f(1.0f, 0.6f, 0.1f);

	      fullOrange = scenario->getMaterial("fullOrange");
        fullOrange.diffuseColor = engine::Vector3f(0.96f, 0.5f, 0.0f);


	      m_front = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, 0.0, -depth/2.0), engine::Vector3f(width, height, arenaThickness),
        engine::Vector3f::Zero(), engine::Quaternion(), engine::Object::TRIGGER_ZONE_0);
	      scenario->setMaterial(m_front, green);
	      scenario->setName(m_front, "front");

	      m_back = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0,  0.0, depth/2.0), engine::Vector3f(width, height, arenaThickness),
        engine::Vector3f::Zero(), engine::Quaternion(), engine::Object::TRIGGER_ZONE_1);
	      scenario->setMaterial(m_back, red);
	      //setName(e, "back");

	      m_left = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(-width/2.0, 0.0, 0.0), engine::Vector3f(arenaThickness, height, depth));





	      //setName(e, "left");

	      m_right = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(width/2.0, 0.0, 0.0), engine::Vector3f(arenaThickness, height, depth));
	      //setName(e, "right");
        switch (level) {
          case 0:
	          scenario->setMaterial(m_left, blue);
	          scenario->setMaterial(m_right, blue);
          break;

          case 1:
            scenario->setMaterial(m_left, darkBlue);
            scenario->setMaterial(m_right, darkBlue);
          break;

          case 2:
            scenario->setMaterial(m_left, purple);
            scenario->setMaterial(m_right, purple);
          break;
        }

	      m_roof = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0.0, height/2.0, 0.0), engine::Vector3f(width, arenaThickness, depth));
	      scenario->setMaterial(m_roof, white);
	      //setName(e, "right");

	      m_floor = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, -height/2.0, 0), engine::Vector3f(width, arenaThickness, depth));
	      scenario->setMaterial(m_floor, white);

        //Entity lamp;
	      //lamp = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS, engine::Vector3f(0, height/2.0-2, 0), engine::Vector3f(4.0f, 4.0f, 4.0f));
	      //scenario->setMaterial(lamp, white);
	      //setName(e, "top");

//===================================================SPIRAL DECORATION=========================================================
        float_t j = 0.0f;
        float_t n = 270.0;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){
          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(-width/2.0f + 0.1f , -height/2.0f + j/9.0f, depth/2.0f - 0.1f), 
            gp::engine::Vector3f(1.0f, 0.1f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion((2*M_PI / 20.0f) * j, engine::Vector3f(0, 1, 0))
            );

	          scenario->setMaterial(e, white);
            j+=1;
        }

        j = 0.0f;
        n = 270.0;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){
          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(width/2.0f - 0.1f , -height/2.0f + j/9.0f, depth/2.0f - 0.1f), 
            gp::engine::Vector3f(1.0f, 0.1f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion((2*M_PI / 20.0f) * j, engine::Vector3f(0, 1, 0))
            );

	          scenario->setMaterial(e, white);
            j+=1;
        }

        j = 0.0f;
        n = 75.0;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){
          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(-width/2.0f + 0.5f , -height/2.0f + j/2.0f, -depth/2.0f + 0.5f), 
            gp::engine::Vector3f(2.0f, 0.3f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion((2*M_PI / 10.0f) * j, engine::Vector3f(0, 1, 0)),
            engine::Object::TRIGGER_ZONE_0
            );

	          scenario->setMaterial(e, white);
            j+=1;
        }

        j = 0.0f;
        n = 75.0f;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){

          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(width/2.0f - 0.5f , -height/2.0f + j/2.0f, -depth/2.0f + 0.5f), 
            gp::engine::Vector3f(2.0f, 0.3f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion((2*M_PI / 10.0f) * j, engine::Vector3f(0, 1, 0)),
            engine::Object::TRIGGER_ZONE_0
            );

	          scenario->setMaterial(e, white);
            j+=1;
        }

        j = 0.0f;
        n = 74.0f;
        std::string colorName;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){
          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(-width/2.0f + 0.4f , -height/2.0f + 0.5f, -depth/2.0f + j/0.75f), 
            gp::engine::Vector3f(3.1f, 0.3f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion((2*M_PI / 6.0f)*j, engine::Vector3f(0, 0, 1))*engine::Quaternion((2*M_PI / 5.0f), engine::Vector3f(0, 1, 0))
            );
            colorName = "c" + std::to_string(j);
            scenario->getMaterial(colorName).diffuseColor = engine::Vector3f(0.2f, 0.5f, j/n);
	          scenario->getMaterial(colorName).shininess = 10.0f;
	          scenario->getMaterial(colorName).specularColor = engine::Vector3f(0.2f, 0.5f, j/40.0f);
	          scenario->setMaterial(e, scenario->getMaterial(colorName));
            j+=1;
        }

        j = 0.0f;
        n = 74.0f;
        for(float_t degree = 0; degree < 2*M_PI; degree += 2*M_PI/n){
          Entity e = scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(width/2.0f - 0.3f , -height/2.0f + 0.5f, -depth/2.0f + j/0.75f), 
            gp::engine::Vector3f(3.1f, 0.3f, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0),
            engine::Quaternion(-(2*M_PI / 6.0f)*j, engine::Vector3f(0, 0, 1))*engine::Quaternion(-(2*M_PI / 5.0f), engine::Vector3f(0, 1, 0))
            );
            colorName = "c" + std::to_string(j);
            scenario->getMaterial(colorName).diffuseColor = engine::Vector3f(0.2f, 0.5f, j/n);
	          scenario->getMaterial(colorName).shininess = 10.0f;
	          scenario->getMaterial(colorName).specularColor = engine::Vector3f(0.2f, 0.5f, j/40.0f);
	          scenario->setMaterial(e, scenario->getMaterial(colorName));
            j+=1;
        }

        scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(-width/2.0f + 0.175 , height/2.0f - 0.175f, 0.0), 
            gp::engine::Vector3f(0.4f, 0.2f, depth),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0)
        );        

        scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(width/2.0f - 0.175 , height/2.0f - 0.175f, 0.0), 
            gp::engine::Vector3f(0.4f, 0.2f, depth),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0)
        );        

        scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(-width/2.0f + 0.175 , height/2.0f - 0.375f, 0.0), 
            gp::engine::Vector3f(0.1f, 0.5f, depth),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0)
        );

        scenario->addBox(gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(width/2.0f - 0.175 , height/2.0f - 0.375f, 0.0), 
            gp::engine::Vector3f(0.1f, 0.5f, depth),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, 0)
        );        



      }

    }
    // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    //m_cameraControl.moveCamera(m_window, m_scenarioControl, *camera);

    if(newLevel) {
      newLevel = false;
      glfwGetCursorPos(m_window, &m_cameraControl.m_lastXPos, &m_cameraControl.m_lastYPos);
    }

    m_cameraControl.moveCamera(m_window, m_scenarioControl, *camera);

    if(!canShoot) {//shot cooldown
      std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
      int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(curTime-lastShot).count();
      canShoot = millis>cooldown;
    }
    //can we shoot currently?
	  if (!(currentlyChargedShooting||currentlyShooting) && canShoot){ 
      if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) { //start a charged shot
        currentlyChargedShooting = true;
        startShot = std::chrono::steady_clock::now();
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 8;
        gp::engine::Vector4f shootColor(255.0f, 0.0f, .0f, 955.0f);
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(shootColor);
      } else if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){ //do a normal shot
        float_t intensityMod = 1.f;
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
        //std::cout << towards << std::endl;
        m_vis2engine.push(gp::engine::messages::ShootMessage(posV,towards,intensityMod));
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 8;
        gp::engine::Vector4f shootColor(255.0f, 0.0f, .0f, 955.0f);
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(shootColor);
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(shootColor);
        lastShot = std::chrono::steady_clock::now();
        canShoot = false;
        cooldown = 250;
        //TODO: add idle color here
        shotIntensityLevel = 0;
        currentlyShooting = true;
      }
    }else if (currentlyShooting){
      std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
      int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(curTime-lastShot).count();
      if(millis>50){
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 4;
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
      currentlyShooting = false;}
    }
    else if (currentlyChargedShooting){
      if (!glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
        currentlyChargedShooting = false;
        float_t intensityMod = 1.f;
        switch (shotIntensityLevel){
          case 0: intensityMod = 1.f;
          break;
          case 1: intensityMod = 2.f;
          break;
          case 2: intensityMod = 3.f;
        }
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
        //std::cout << towards << std::endl;
        m_vis2engine.push(gp::engine::messages::ShootMessage(posV,towards,intensityMod));
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowCornerRadius = 4;
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillUnfocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
        m_scenarioControl.m_aimingReticleHor->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
        m_scenarioControl.m_aimingReticleVer->theme()->mWindowFillFocused = nanogui::Color(engine::Vector4f(255.0f, 255.0f, 255.0f, 255.0f));
        lastShot = std::chrono::steady_clock::now();
        canShoot = false;
        //TODO: add idle color here
        shotIntensityLevel = 0;
        cooldown = 1000;
      }else{
        std::chrono::steady_clock::time_point curTime = std::chrono::steady_clock::now();
        int64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(curTime-startShot).count();
        if(millis>1000){
          shotIntensityLevel = 1;//TODO: add charge level 1 color here
        }
        if(millis>2500){
          shotIntensityLevel = 2;//TODO: add charge level 2 color here
        }
      }
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
      if(numObjectsToDespawn <= 9) {
        if(level >= 2){            
          m_scenarioControl.m_winlose->setCaption("YOU WIN \n");
          m_scenarioControl.m_endscore->setCaption("SCORE:  "+ std::to_string(score));
          m_scenarioControl.m_endmenu->setVisible(true);
          m_scenarioControl.toggleEngine();
        }
        else{
          m_scenarioControl.loadScenario<gp::Custom3>();
          m_scenarioControl.m_lifeLabel->setCaption("LIVES: "+ std::to_string(lives));
          m_scenarioControl.m_scoreLabel->setCaption("SCORE: "+ std::to_string(score));

          newLevel = true;
          switch (level) {
            case 0: // 0->1 transition
              numObjectsToDespawn = 15;
              numObjectsToSpawn = 15;
              levelVel = 2;

            break;
            case 1: //1->2 transition
              numObjectsToDespawn = 20;
              numObjectsToSpawn = 20;
              levelVel = 4;
            break;
          }
          level += 1;
          m_scenarioControl.m_moobsLabel->setCaption("ENEMIES: "+ std::to_string(numObjectsToDespawn));
          m_scenarioControl.m_levelLabel->setCaption("LEVEL: "+ std::to_string(level+1));
          spawnIndex = 0;
          continue;
        }
      }
      if(!m_scenarioControl.isPaused()){
        for(int ii = 0; ii < spawnIndex; ii++){
	        engine::Object* o = scenario->engineObjectManager().find(spawnedObjects[ii]);
          int shots = o->numShots();
          switch (shots) {
            case 0:

              if (dynamic_cast<engine::Box*>(o) != nullptr){
                scenario->setMaterial(spawnedObjects[ii], fullOrange);
              }
              else{
                scenario->setMaterial(spawnedObjects[ii], fullRed);
              }
            break;

            case 1:
              if (dynamic_cast<engine::Box*>(o) != nullptr){
                scenario->setMaterial(spawnedObjects[ii], orange);
              }
              else{
                scenario->setMaterial(spawnedObjects[ii], red);
              }
            break;

            case 2:
              if (dynamic_cast<engine::Box*>(o) != nullptr){
                scenario->setMaterial(spawnedObjects[ii], lightOrange);
              }
              else{
                scenario->setMaterial(spawnedObjects[ii], lightRed);
              }
            break;
          }
        }

        long int t1 = static_cast<long int> (std::time(NULL));
        if (t1 - t0 > 0.3f) {
          t0 = static_cast<long int> (std::time(NULL));
          scenario->setMaterial(m_front, green);
          scenario->setMaterial(m_back, red);
          wave+=1;
        }
        if (t1 - t0_ball > 5.0f && numObjectsToSpawn > 0) {
          t0_ball = static_cast<long int> (std::time(NULL));
          float rPos_x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (width -4.0f) - (width/2.0f-2.0f);
          float rPos_y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (height-4.0f) - (height/2.0f-2.0f);

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
        spawnedObjects[spawnIndex] = scenario->addSphere(1.0f, engine::Vector3f(rPos_x, rPos_y, rPos_z + 5.5f), rRadius, 
                              levelVel*rVel_length*engine::Vector3f(rVel_x+rVel_z, rVel_y, rVel_z+5.5f));


        numObjectsToSpawn -= 1;
	      gp::graphics::Material& redPink = scenario->getMaterial("redPink");
       	redPink.diffuseColor = engine::Vector3f(0.9f, 0.1f, 0.4f);

	      gp::graphics::Material& orangeYellow = scenario->getMaterial("orangeYellow");
       	orangeYellow.diffuseColor = engine::Vector3f(0.95f, 0.50f, 0.0f);


        scenario->setMaterial(spawnedObjects[spawnIndex], fullRed);
        //int color = rand() % 3 + 1;  
        //if (color == 1){
        //  scenario->setMaterial(spawnedObjects[spawnIndex], redPink);
        //}
        //if (color == 2){
        //  scenario->setMaterial(spawnedObjects[spawnIndex], orangeYellow);
        //}
        //if (color == 3){
        //  //scenario->setMaterial(nBall, white);
        //}

        spawnIndex+=1;
        }

        if (t1 - t0_squares > 15.0f && numObjectsToSpawn > 0) {
          t0_squares= static_cast<long int> (std::time(NULL));
          float rPos_x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (width -6.0f) - (width/2.0f-3.0f);
          float rPos_y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * (height-6.0f) - (height/2.0f-3.0f);
          float rWidth = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*4.0f + 4.0f; 
          float rHeight = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) * 1.0f + 1.0f;

          //from -depth/2 to depth/2 - 5
          float rVel_z = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX))*10.0f + 10.0f;

          float rRadius = static_cast <float> (rand()) / static_cast <float> (RAND_MAX) + 0.5f;

          spawnedObjects[spawnIndex] = scenario->addBox(1.0f,//gp::engine::Object::UNMOVABLE_MASS,
            engine::Vector3f(0, rPos_y, -depth/2.0f + 3.0f), 
            gp::engine::Vector3f(rWidth, rHeight, 0.1f),//gp::engine::Vector3f(0.1f, 0.1f, 0.1f),
            //engine::Vector3f(2.0f, 0.1f, 0.2f),
            engine::Vector3f(0, 0, levelVel*rVel_z)
            );        


          numObjectsToSpawn -= 1;

	        engine::Object* o = scenario->engineObjectManager().find(spawnedObjects[spawnIndex]);
	        o->setAngularVelocity(engine::Vector3f(0, 0, 7*M_PI));
          o->setVelocity(engine::Vector3f(0,0,100.0f));
          scenario->setMaterial(spawnedObjects[spawnIndex], fullOrange);
          spawnIndex+=1;

        //int color = rand() % 3 + 1;  
        //if (color == 1){
        //  scenario->setMaterial(nBall, redPink);
        //}
        //if (color == 2){
        //  scenario->setMaterial(nBall, orangeYellow);
        //}
        //if (color == 3){
        //  //scenario->setMaterial(nBall, white);
        //}


          wave+=1;
        }
      }
      else{
          //5 and 0.5 seconds after the pause, we will have the events, if there's no another pause in between.
          t0 = static_cast<long int> (std::time(NULL));
          t0_ball = static_cast<long int> (std::time(NULL));
          t0_squares = static_cast<long int> (std::time(NULL));
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
        lives -= 1;
        m_scenarioControl.m_lifeLabel->setCaption("LIVES: "+ std::to_string(lives));
        scenario->setMaterial(m_back, fullRed);
        numObjectsToDespawn -=1;

        //Game Over
        // if(lives <= 0){
        
        //   m_scenarioControl.m_winlose->setCaption("GAME OVER ");
        //   m_scenarioControl.m_endscore->setCaption("SCORE:  "+ std::to_string(score));
        //   m_scenarioControl.m_endmenu->setVisible(true);
        //   m_scenarioControl.toggleEngine();
        // }

      }
      if (gp::messages::isType<gp::engine::messages::EnemyDeathByGoalMessage>(message))
      {
        m_scenarioControl.m_score += 1;
        score += 100;
        m_scenarioControl.m_scoreLabel->setCaption("SCORE:  "+ std::to_string(score));
        scenario->setMaterial(m_front, fullGreen);
        numObjectsToDespawn -=1;
        m_scenarioControl.m_moobsLabel->setCaption("ENEMIES:  "+ std::to_string(numObjectsToDespawn));
      }

      if (gp::messages::isType<gp::engine::messages::EnemyDeathByRayMessage>(message))
      {
        score += 10;
        m_scenarioControl.m_scoreLabel->setCaption("SCORE:  "+ std::to_string(score));
        numObjectsToDespawn -=1;
        m_scenarioControl.m_moobsLabel->setCaption("ENEMIES:  "+ std::to_string(numObjectsToDespawn));
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
