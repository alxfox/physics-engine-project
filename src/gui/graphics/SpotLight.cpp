#include "SpotLight.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

gp::graphics::SpotLight::SpotLight()
{  
  setupLight(glm::vec3(0.0f, 3.0f, 2.0f), glm::vec3(0.0f, -1.0f, -0.5f), 45.0f, 0.1f);
  glGenBuffers(1, &ubo);
  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferData(GL_UNIFORM_BUFFER, 8*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

gp::graphics::SpotLight::~SpotLight()
{
  glDeleteBuffers(1, &ubo);
}

void gp::graphics::SpotLight::setupLight(glm::vec3 const& position, glm::vec3 const& spotDirection, GLfloat cutoffAngle, GLfloat ambientIntensity)
{
  m_position = position;
  m_spotDirection = spotDirection;
  m_cutoffAngle = glm::cos(glm::radians(cutoffAngle));
  m_ambientIntensity = ambientIntensity;

  glm::mat4 depthProjectionMatrix = glm::perspective<float>(glm::radians(2.0f*cutoffAngle), 1.0f, 1.0f, 70.0f);
  glm::mat4 depthViewMatrix = glm::lookAt(m_position, m_position + m_spotDirection, glm::vec3(0.0f, 0.0f, -1.0f));
  depthVP = depthProjectionMatrix * depthViewMatrix;
}

void gp::graphics::SpotLight::update(glm::mat4 const& view)
{
  glm::vec3 position = view * glm::vec4(m_position, 1.0);
  glm::vec3 spotDirection = glm::mat3(view) * m_spotDirection;

  glBindBuffer(GL_UNIFORM_BUFFER, ubo);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, 3*sizeof(GLfloat), glm::value_ptr(position));
  glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(GLfloat), sizeof(GLfloat), &m_cutoffAngle);
  glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(GLfloat), 3*sizeof(GLfloat), glm::value_ptr(spotDirection));
  glBufferSubData(GL_UNIFORM_BUFFER, 7*sizeof(GLfloat), sizeof(GLfloat), &m_ambientIntensity);
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


