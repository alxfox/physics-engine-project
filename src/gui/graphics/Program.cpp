#include "Program.h"

#include <iostream>

void gp::graphics::Program::compileShader(GLuint shader, std::string const& shaderSource)
{
	GLchar const* source = shaderSource.c_str();
	GLint length = shaderSource.size();

  glShaderSource(shader, 1, &source, &length);
  glCompileShader(shader);

  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  if (compileStatus != GL_TRUE) {
    GLint logLength;
    GLchar* log;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
    log = new GLchar[logLength+1];
    log[logLength] = '\0';
    glGetShaderInfoLog(shader, logLength, NULL, log);
    if (logLength > 0) {
      std::cerr << log << std::endl << shaderSource << std::endl;
    }
    delete[] log;
  }
}

void gp::graphics::Program::link()
{
  m_program = glCreateProgram();
  glAttachShader(m_program, m_vshader);
  glAttachShader(m_program, m_fshader);
  glLinkProgram(m_program);
  
  GLint compileStatus;
  glGetProgramiv(m_program, GL_LINK_STATUS, &compileStatus);
  if (compileStatus != GL_TRUE) {
    GLint logLength;
    GLchar* log;
    glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &logLength);
    log = new GLchar[logLength + 1];
    log[logLength] = '\0';
    glGetProgramInfoLog(m_program, logLength, NULL, log);
    std::cerr << log << std::endl;
    delete[] log;
  }
}

gp::graphics::Program::~Program()
{
  glDeleteShader(m_vshader);
  glDeleteShader(m_fshader);
  glDeleteProgram(m_program);
}

void gp::graphics::Program::compileAndLink(std::string const& vertexShader, std::string const& fragmentShader)
{
  m_vshader = glCreateShader(GL_VERTEX_SHADER);
  compileShader(m_vshader, vertexShader);

  m_fshader = glCreateShader(GL_FRAGMENT_SHADER);
  compileShader(m_fshader, fragmentShader);
  
  link();
  
  ubo.Camera    = glGetUniformBlockIndex(m_program, "Camera");
  ubo.SpotLight = glGetUniformBlockIndex(m_program, "SpotLight");
  ubo.Material  = glGetUniformBlockIndex(m_program, "Material");

  uni.NormalMatrix  = glGetUniformLocation(m_program, "NormalMatrix");
  uni.MV            = glGetUniformLocation(m_program, "MV");
  uni.depthMVP      = glGetUniformLocation(m_program, "depthMVP");
  uni.ShadowMap     = glGetUniformLocation(m_program, "ShadowMap");
  uni.Textures      = glGetUniformLocation(m_program, "Textures");
}

void gp::graphics::Program::use()
{
  glUseProgram(m_program);
}
