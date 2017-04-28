#include "Material.h"

template <class T>
void hash_combine(std::size_t& seed, const T& v)
{
	std::hash<T> hasher;
	seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

namespace std {

template<> struct hash<gp::engine::Vector3f>
{
	/**
	 * @return The hash code for a {@link gp::engine::Vector3f}
	 */
	size_t operator()(gp::engine::Vector3f const & v) const
	{
		size_t seed = 0;
		std::hash<gp::engine::float_t> hasher;
		hash_combine(seed, hasher(v(0)));
		hash_combine(seed, hasher(v(1)));
		hash_combine(seed, hasher(v(2)));
		return seed;
	}
};

}

gp::graphics::Material::Material()
{
	diffuseColor  = engine::Vector3f::Ones();
	shininess     = 25.0f;
	specularColor = engine::Vector3f::Ones() * 0.2f;

	for (unsigned t = 0; t < NUMBER_OF_TEXTURE_TYPES; ++t) {
		m_textures[t] = 0;
		m_textureHashes[t] = 0;
	}

	glGenBuffers(1, &m_ubo);
}

gp::graphics::Material::~Material()
{
	glDeleteBuffers(1, &m_ubo);
}

void gp::graphics::Material::setTexture(enum TextureType type, std::string const& name)
{
	m_textureNames[type] = name;
	m_textureHashes[type] = 0;
	hash_combine(m_textureHashes[type], static_cast<unsigned>(type));
	hash_combine(m_textureHashes[type], name);
	if (m_textureNames[type].size() > 0) {
		m_activeTextures |= (1 << type);
	}
}

void gp::graphics::Material::bind(TextureManager const& textureMan, GLuint bindingPoint, GLenum firstTextureUnit)
{
	size_t hash = 0;
	hash_combine(hash, diffuseColor);
	hash_combine(hash, shininess);
	hash_combine(hash, specularColor);
	for (unsigned t = 0; t < NUMBER_OF_TEXTURE_TYPES; ++t) {
		hash_combine(hash, m_textureHashes[t]);
	}

	if (hash != m_hash) {
		m_hash = hash;
		glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
		glBufferData(GL_UNIFORM_BUFFER, 8*sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, 3*sizeof(GLfloat), diffuseColor.data());
		glBufferSubData(GL_UNIFORM_BUFFER, 3*sizeof(GLfloat), sizeof(GLfloat), &shininess);
		glBufferSubData(GL_UNIFORM_BUFFER, 4*sizeof(GLfloat), 3*sizeof(GLfloat), specularColor.data());
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		for (unsigned t = 0; t < NUMBER_OF_TEXTURE_TYPES; ++t) {
			if (m_textureNames[t].size() > 0) {
				m_textures[t] = textureMan.texture(m_textureNames[t]);
			}
		}
	}

	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_ubo);
	for (unsigned t = 0; t < NUMBER_OF_TEXTURE_TYPES; ++t) {
		if (m_textures[t] > 0) {
			glActiveTexture(firstTextureUnit + t);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[t]);
		}
	}
}
