#include "TextureManager.h"
#include <common/Dir.h>
// Implementation already created in nanovg
//#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>
#include <iostream>

std::string basename(std::string const& name) {
  size_t dot = name.find_last_of(".");
  return name.substr(0, dot);
}

gp::graphics::TextureManager::~TextureManager()
{
  for (auto it = m_textures.begin(); it != m_textures.end(); ++it) {
    glDeleteTextures(1, &it->second);
  }
}

void gp::graphics::TextureManager::addDirectory(std::string const& textureDir)
{
  gp::Dir dir(textureDir);
  std::vector<std::string> files = dir.list(gp::Dir::Files);
  struct {
    bool operator()(std::string const& a, std::string const& b) {
      return basename(a) < basename(b);
    }   
  } customLess;
  std::sort(files.begin(), files.end(), customLess);
  for (auto it = files.begin(); it != files.end(); ++it) {
    std::string path(textureDir + "/" + *it);
    std::string name = basename(*it);
    addTexture(name, path);
  }
}

void gp::graphics::TextureManager::addTexture(std::string name, std::string const& path)
{
  size_t dot = name.find_last_of(".");
  int side = (dot != std::string::npos && dot+1 < name.size()) ? std::stoi(name.substr(dot+1)) : 6;
  bool sideValid = (side >= 0 && side <= 5);
  if (sideValid) {
    name = name.substr(0, dot);
  }
    
  GLuint tex;
  if (m_textures.find(name) == m_textures.end()) {
    glGenTextures(1, &tex);
    m_textures[name] = tex;
  } else {
    tex = m_textures[name];
  }
  
  int w, h, n;
  unsigned char* data = stbi_load(path.c_str(), &w, &h, &n, 3);
  if (data == NULL) {    
    std::cerr << "An error occured while loading " << path << "." << std::endl;
    return;
  }
  
  glBindTexture(GL_TEXTURE_CUBE_MAP, tex);
  GLuint begin, end;
  begin = (sideValid) ? side : 0;
  end = (sideValid) ? side : 5;
  for(GLuint i = begin; i <= end; ++i) {
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
  glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
  
  stbi_image_free(data);
}
