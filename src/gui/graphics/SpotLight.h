#ifndef GP_GUI_GRAPHICS_SPOTLIGHT_H
#define GP_GUI_GRAPHICS_SPOTLIGHT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gp
{
namespace graphics
{

class SpotLight
{
private:
  glm::vec3 m_position;
  glm::vec3 m_spotDirection;
  GLfloat m_cutoffAngle;
  GLfloat m_ambientIntensity;

public:
  glm::mat4 depthVP;
  GLuint ubo;

  SpotLight();
  ~SpotLight();
  
  void setupLight(glm::vec3 const& position, glm::vec3 const& spotDirection, GLfloat cutoffAngle, GLfloat ambientIntensity);
  void update(glm::mat4 const& view);
  
  glm::vec3 position() const { return m_position; }
  glm::vec3 spotDirection() const { return m_spotDirection; }
  GLfloat cutoffAngle() const { return m_cutoffAngle; }
  GLfloat ambientIntensity() const { return m_cutoffAngle; }
};

}
}

#endif // GP_GUI_GRAPHICS_SPOTLIGHT_H
