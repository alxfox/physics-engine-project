#include "Render.h"

#include <cmath>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

gp::graphics::Render::Render()
{
  // GL initialization
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glClearColor(0.2f, 0.25f, 0.3f, 1.0f);
  glProvokingVertex(GL_FIRST_VERTEX_CONVENTION);
  glEnable(GL_FRAMEBUFFER_SRGB);
  
  // Shadow maps
  glGenFramebuffers(1, &m_framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

  glGenTextures(1, &m_shadowMap);
  glBindTexture(GL_TEXTURE_2D, m_shadowMap);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, ShadowMapSize, ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
  GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
  glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
  
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_shadowMap, 0);
  glDrawBuffer(GL_NONE);
  glReadBuffer(GL_NONE);
  
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    std::cerr << "Something is wrong with the framebuffer." << std::endl;
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  
  glGenBuffers(1, &m_camera_ubo);
  
  // Performance counter
  glGenQueries(1, &performanceQuery);
  
  // Setup lines
  glGenBuffers(1, &m_line_vbo);
  glGenVertexArrays(1, &m_line_vao);  
  glBindVertexArray(m_line_vao);
  glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
  glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

gp::graphics::Render::~Render()
{
  glDeleteFramebuffers(1, &m_framebuffer);
  glDeleteTextures(1, &m_shadowMap);
  glDeleteBuffers(1, &m_camera_ubo);
  glDeleteQueries(1, &performanceQuery);
  glDeleteBuffers(1, &m_line_vbo);
  glDeleteVertexArrays(1, &m_line_vao);
}

void gp::graphics::Render::updateConstraints(ConstraintManager const& constraintMan)
{
  m_numberOfLines = constraintMan.data.size();
  glBindBuffer(GL_ARRAY_BUFFER, m_line_vbo);
  glBufferData(GL_ARRAY_BUFFER, 3 * 2 * m_numberOfLines * sizeof(GLfloat), &constraintMan.data.line[0].start, GL_DYNAMIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

unsigned gp::graphics::Render::renderShadowMap(RenderComponentManager const& renderComponentMan, unsigned start, Shape shape)
{
  shapes.bind(shape);
  unsigned i = start;
  for (; i < renderComponentMan.data.size() && renderComponentMan.data.shape[i] == shape; ++i) {
    glUniformMatrix4fv(programs.shadow.uni.depthMVP, 1, GL_FALSE, glm::value_ptr(renderComponentMan.data.depthMVP[i]));
    shapes.draw(shape);
  }
  return i;
}

unsigned gp::graphics::Render::renderObjects(RenderComponentManager const& renderComponentMan, TextureManager const& textureMan, unsigned start, Shape shape)
{
  enum NormalInterpolation interpolationType = (shape == Cube) ? Flat : Smooth;
  shapes.bind(shape);
  
  size_t lastActiveTextures = std::numeric_limits<size_t>::max();
  Program* program = NULL;

  unsigned i = start;
  for (; i < renderComponentMan.data.size() && renderComponentMan.data.shape[i] == shape; ++i) {
    size_t activeTextures = renderComponentMan.data.material[i]->activeTextures();
    if (activeTextures != lastActiveTextures) {
      program = programs.shading(interpolationType, activeTextures);
      program->use();

      glUniform1i(program->uni.ShadowMap, 0);
      program->setBlockBinding(program->ubo.Camera, CameraBP);
      program->setBlockBinding(program->ubo.SpotLight, SpotLightBP);
      program->setBlockBinding(program->ubo.Material, MaterialBP);

      GLint textureUnits[Material::NUMBER_OF_TEXTURE_TYPES];
      for (unsigned t = 0; t < Material::NUMBER_OF_TEXTURE_TYPES; ++t) {
        textureUnits[t] = 1 + t;
      }
      glUniform1iv(program->uni.Textures, Material::NUMBER_OF_TEXTURE_TYPES, textureUnits);
    }
    
    renderComponentMan.data.material[i]->bind(textureMan, MaterialBP, GL_TEXTURE1);
    glUniformMatrix3fv(program->uni.NormalMatrix, 1, GL_FALSE, glm::value_ptr(renderComponentMan.data.normalMatrix[i]));
    glUniformMatrix4fv(program->uni.MV, 1, GL_FALSE, glm::value_ptr(renderComponentMan.data.MV[i]));
    glUniformMatrix4fv(program->uni.depthMVP, 1, GL_FALSE, glm::value_ptr(renderComponentMan.data.depthMVP[i]));

    shapes.draw(shape);
  }
  
  return i;
}

void gp::graphics::Render::renderConstraintsShadow(SpotLight const& spotLight)
{  
  glBindVertexArray(m_line_vao);
  glUniformMatrix4fv(programs.shadow.uni.depthMVP, 1, GL_FALSE, glm::value_ptr(spotLight.depthVP));
  glDrawArrays(GL_LINES, 0, 2 * m_numberOfLines);
}

void gp::graphics::Render::renderConstraints()
{  
  programs.lines.use();
  glBindVertexArray(m_line_vao);
  programs.lines.setBlockBinding(programs.lines.ubo.Camera, CameraBP);
  glDrawArrays(GL_LINES, 0, 2 * m_numberOfLines);
}

void gp::graphics::Render::render(  RenderComponentManager const& renderComponentMan, 
                                    ConstraintManager const& constraintMan,
                                    Camera const& camera,
                                    SpotLight& spotLight,
                                    TextureManager const& textureMan,
                                    bool enableConstraintsRendering  )
{
  glBeginQuery(GL_TIME_ELAPSED, performanceQuery);
  glEnable(GL_DEPTH_TEST);
  
  // Projection matrix
  glBindBuffer(GL_UNIFORM_BUFFER, m_camera_ubo);
  glBufferData(GL_UNIFORM_BUFFER, 32*sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
  glBufferSubData(GL_UNIFORM_BUFFER, 0, 16*sizeof(GLfloat), glm::value_ptr(camera.projection));
  glBufferSubData(GL_UNIFORM_BUFFER, 16*sizeof(GLfloat), 16*sizeof(GLfloat), glm::value_ptr(camera.view));
  glBindBuffer(GL_UNIFORM_BUFFER, 0);
  
  if (enableConstraintsRendering) {
    updateConstraints(constraintMan);
  }
  
  // Shadow map
  // ==========
  programs.shadow.use();
  glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
  glViewport(0, 0, ShadowMapSize, ShadowMapSize);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(2.0f, 4.0f);
  glClear(GL_DEPTH_BUFFER_BIT);

  unsigned start = 0;
  start = renderShadowMap(renderComponentMan, start, Cube);
  start = renderShadowMap(renderComponentMan, start, Sphere);
  if (enableConstraintsRendering) {
    renderConstraintsShadow(spotLight);
  }
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glDisable(GL_POLYGON_OFFSET_FILL);
  
  // Lighting
  // ========
  glBindBufferBase(GL_UNIFORM_BUFFER, CameraBP, m_camera_ubo);

  spotLight.update(camera.view);
  glBindBufferBase(GL_UNIFORM_BUFFER, SpotLightBP, spotLight.ubo);
  
  glViewport(0, 0, camera.width(), camera.height());
  glPolygonOffset(0.0, 0.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // Bind texture map to slot 0
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m_shadowMap);

  start = 0;
  start = renderObjects(renderComponentMan, textureMan, start, Cube);
  start = renderObjects(renderComponentMan, textureMan, start, Sphere);
  
  // Constraints
  // ===========
  if (enableConstraintsRendering) {
    renderConstraints();
  }
  
  
  glEndQuery(GL_TIME_ELAPSED);
}

GLuint64 gp::graphics::Render::lastGPUTime()
{
  GLint available = 0;
  while (!available) {
    glGetQueryObjectiv(performanceQuery, GL_QUERY_RESULT_AVAILABLE, &available);
  }
  GLuint64 time;
  glGetQueryObjectui64v(performanceQuery, GL_QUERY_RESULT, &time);
  return time;
}
