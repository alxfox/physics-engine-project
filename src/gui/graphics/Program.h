#ifndef GP_GUI_GRAPHICS_PROGRAM_H
#define GP_GUI_GRAPHICS_PROGRAM_H

#include <glad/glad.h>
#include <string>

namespace gp
{
namespace graphics
{

class Program
{
private:
	void compileShader(GLuint shader, std::string const& shaderSource);
  void link();

  GLuint m_vshader = 0;
  GLuint m_fshader = 0;
  GLuint m_program = 0;
public:  
  struct {
    GLuint Camera, SpotLight, Material;
  } ubo;

  struct {
    GLint NormalMatrix, MV, depthMVP, ShadowMap, Textures;
  } uni;
  
  Program() {}
  ~Program();
  
  void compileAndLink(std::string const& vertexShader, std::string const& fragmentShader);
  void setBlockBinding(GLuint uboIndex, GLuint bindingPoint) {
    glUniformBlockBinding(m_program, uboIndex, bindingPoint);
  }
  void use();
};

}
}

#endif // GP_GUI_GRAPHICS_PROGRAM_H
