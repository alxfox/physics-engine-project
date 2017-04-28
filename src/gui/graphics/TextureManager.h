#ifndef GP_GUI_GRAPHICS_TEXTUREMANAGER_H
#define GP_GUI_GRAPHICS_TEXTUREMANAGER_H

#include <glad/glad.h>
#include <string>
#include <unordered_map>
#include <iostream>

namespace gp
{
  namespace graphics
  {
    class TextureManager {
    private:
      std::unordered_map<std::string, GLuint> m_textures;
      
      void addTexture(std::string name, std::string const& path);

    public:
      TextureManager() {}
      ~TextureManager();
      void addDirectory(std::string const& textureDir);
      GLuint texture(std::string const& name) const {
        auto it = m_textures.find(name);
        if (it == m_textures.end()) {
          std::cerr << "Unable to retrieve texture " << name << "." << std::endl;
          return 0u;
        }
        return it->second;
      }
    };
  }
}

#endif // GP_GUI_GRAPHICS_TEXTUREMANAGER_H
