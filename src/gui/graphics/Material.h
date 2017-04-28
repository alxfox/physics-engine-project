#ifndef GP_GUI_GRAPHICS_MATERIAL_H
#define GP_GUI_GRAPHICS_MATERIAL_H

#include <glad/glad.h>
#include <string>
#include <engine/utils.h>
#include "TextureManager.h"

namespace gp
{

namespace graphics
{

class Material
{
public:
	enum TextureType {
		Diffuse,
		NormalMap,
		NUMBER_OF_TEXTURE_TYPES
	};

private:
	size_t m_hash = 0;
	GLuint m_ubo = 0;
	GLuint m_textures[NUMBER_OF_TEXTURE_TYPES];
	size_t m_textureHashes[NUMBER_OF_TEXTURE_TYPES];
	std::string m_textureNames[NUMBER_OF_TEXTURE_TYPES];
	size_t m_activeTextures = 0;

public:
	engine::Vector3f diffuseColor;
	GLfloat   shininess;
	engine::Vector3f specularColor;
	
	Material();
	~Material();

	void setTexture(enum TextureType type, std::string const& name);
	size_t activeTextures() const { return m_activeTextures; }

	void bind(TextureManager const& textureMan, GLuint bindingPoint, GLenum firstTextureUnit);
};

}

}

#endif // GP_GUI_GRAPHICS_MATERIAL_H
