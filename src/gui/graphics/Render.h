#ifndef GP_GUI_GRAPHICS_RENDER_H
#define GP_GUI_GRAPHICS_RENDER_H

#include "Camera.h"
#include "Program.h"
#include "ProgramLibrary.h"
#include "ConstraintManager.h"
#include "RenderComponentManager.h"
#include "ShapeLibrary.h"
#include "SpotLight.h"
#include "TextureManager.h"

namespace gp
{
namespace graphics
{
class Render
{
private:
  static unsigned const ShadowMapSize = 1024;
  
  enum BindingPoints {
    CameraBP = 1,
    SpotLightBP,
    MaterialBP
  };

  ShapeLibrary shapes;
  ProgramLibrary programs;

  GLuint m_camera_ubo;
  GLuint m_shadowMap;
  GLuint m_framebuffer;
  GLuint performanceQuery;
  GLuint m_line_vbo;
  GLuint m_line_vao;
  GLuint m_numberOfLines = 0;
  
  void updateConstraints(ConstraintManager const& constraintMan);
  
  unsigned renderShadowMap(RenderComponentManager const& renderComponentMan, unsigned start, Shape shape);  
  unsigned renderObjects(RenderComponentManager const& renderComponentMan, TextureManager const& textureMan, unsigned start, Shape shape);
  void renderConstraintsShadow(SpotLight const& spotLight);
  void renderConstraints();

public:
  Render();
  ~Render();

  void render(  RenderComponentManager const& renderComponentMan,
                ConstraintManager const& constraintMan,
                Camera const& camera,
                SpotLight& spotLight,
                TextureManager const& textureMan,
                bool enableConstraintsRendering  );
  GLuint64 lastGPUTime();
};

}
}

#endif // GP_GUI_GRAPHICS_RENDER_H
