#ifndef GP_GUI_CAMERACONTROL_H
#define GP_GUI_CAMERACONTROL_H

#include "ScenarioControl.h"
#include "graphics/Camera.h"

struct GLFWwindow;
namespace gp {
  namespace gui {
    class CameraControl {
    private:
      double m_lastXPos = 0.0;
      double m_lastYPos = 0.0;
      float m_xrot = 0.0f;
      float m_yrot = 0.0f;
    public:
      void moveCamera(GLFWwindow* window, gp::gui::ScenarioControl& scenarioControl, gp::graphics::Camera& camera);
      void reset();
    };
  }
}

#endif // GP_GUI_CAMERACONTROL_H
