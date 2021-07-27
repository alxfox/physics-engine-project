#include "iostream"
#include "CameraControl.h"
#include <GLFW/glfw3.h>

bool inside = false;

void gp::gui::CameraControl::moveCamera(GLFWwindow* window, gp::gui::ScenarioControl& scenarioControl, gp::graphics::Camera& camera)
{
  //if (scenarioControl.interactsWithMouse()) {
  //  inside = false;
  //  return;
  //}
	int width, height;
	glfwGetWindowSize(window, &width, &height);
  
  scenarioControl.m_aimingReticleHor->setPosition(Eigen::Vector2i(width/2.0f-20, height/2.0f));
  scenarioControl.m_aimingReticleVer->setPosition(Eigen::Vector2i(width/2.0f-9, height/2.0f-9));
  scenarioControl.m_nanoguiWindow->setPosition(Eigen::Vector2i(width/2.0f- (width/ 5.0f)/2.0f, height/2.0f -(height / 5.0f)/2.0f ));
  scenarioControl.m_nanoguiWindow->setSize(Eigen::Vector2i(width / 5.0f, height / 5.0f));
  int menuWidth = scenarioControl.m_nanoguiWindow->width();
  int menuHeight = scenarioControl.m_nanoguiWindow->height();
  //scenarioControl.m_g1->setPosition(Eigen::Vector2i(0, menuHeight/2 - 3*menuHeight/13-menuHeight/18));
  scenarioControl.m_g2->setPosition(Eigen::Vector2i(0, menuHeight/2 - 2*menuHeight/6));
  scenarioControl.m_g3->setPosition(Eigen::Vector2i(0, menuHeight/2 - menuHeight/6+menuHeight/9));
  //scenarioControl.m_g1->setSize(Eigen::Vector2i(menuWidth/1 -40, menuHeight/8));
  scenarioControl.m_g2->setSize(Eigen::Vector2i(menuWidth/1 -40, menuHeight/4));
  scenarioControl.m_g3->setSize(Eigen::Vector2i(menuWidth/1 -40, menuHeight/4));
  scenarioControl.m_scoringboard->setPosition(Eigen::Vector2i(width-200, 1));
  if (scenarioControl.isPaused()) {
    inside = false;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
    return;
  }
  

   double xpos, ypos;
   glfwGetCursorPos(window, &xpos, &ypos);
   if (!inside && xpos > 0 && ypos >0 && xpos < width && ypos <height){
     inside = true;
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); 
     glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
     glfwGetCursorPos(window, &m_lastXPos, &m_lastYPos);
     return;
   }
   if (xpos < 0 || ypos < 0 || xpos > width || ypos > height){
     inside = false;
     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
     return;
   }
   if (inside && xpos > width/2 + width/4 || xpos < width/2 - width/4 || ypos > height/2 + height/4 || ypos < height/2 - height/4){
     glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
     glfwGetCursorPos(window, &m_lastXPos, &m_lastYPos);
   }
	//if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
    //double xpos, ypos;

    glfwGetCursorPos(window, &xpos, &ypos);
		m_xrot += (xpos-m_lastXPos) / (0.1f * camera.width());
		m_yrot += (ypos-m_lastYPos) / (0.1f * camera.height());
	//}
  glfwGetCursorPos(window, &m_lastXPos, &m_lastYPos);


  int W = (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) ? 1 : 0;
  int A = (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) ? 1 : 0;
  int S = (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) ? 1 : 0;
  int D = (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) ? 1 : 0;
  int E = (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) ? 1 : 0;
  int Q = (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) ? 1 : 0;

  float speed = 0.05f;
  //camera.translate(glm::vec3( (A-D) * speed, 0*(Q-E) * speed, (W-S) * speed ));
  float xFactor = 4.5f;
  float yFactor = 14.5f;

    
  camera.translate(glm::vec3( (A-D) * speed, -(W-S) * speed, 0.0*(Q-E) * speed ));
  //translateCC -> it is modified so that you can't get off the floor

  if(camera.worldPosition().x >= xFactor){
   glm::vec3 rectifier = camera.worldPosition();
   rectifier.x = xFactor;
   camera.setWorldPosition(rectifier);
  }

  if(camera.worldPosition().x <= -xFactor){
   glm::vec3 rectifier = camera.worldPosition();
   rectifier.x = -xFactor;
   camera.setWorldPosition(rectifier);
  }

  if(camera.worldPosition().y >= yFactor){
   glm::vec3 rectifier = camera.worldPosition();
   rectifier.y = yFactor;
   camera.setWorldPosition(rectifier);
  }

  if(camera.worldPosition().y <= -yFactor){
   glm::vec3 rectifier = camera.worldPosition();
   rectifier.y = -yFactor;
   camera.setWorldPosition(rectifier);
  }

  glm::vec3 yAxis = glm::vec3(0.0, 1.0, 0.0);
  glm::vec3 xAxis = glm::vec3(1.0, 0.0, 0.0);
	glm::quat rot1 = glm::rotate(glm::quat(), m_yrot, xAxis);
	glm::quat rot2 = glm::rotate(rot1, m_xrot, yAxis);
	camera.setRotation(rot2);
}

void gp::gui::CameraControl::reset()
{
  m_lastXPos = 0.0;
  m_lastYPos = 0.0;
  m_xrot = 0.0f;
  m_yrot = 0.0f;
}
