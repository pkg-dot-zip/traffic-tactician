#pragma once

#include <gl/glew.h>
#include <string>
#include <glm/vec3.hpp>


class Texture
{
	GLuint id;
	Texture(const std::string& fileName);
	Texture(const glm::vec3& color = glm::vec3(1.0f));
public:
	~Texture();
	void bind();
	void unbind();

	friend class TextureCache;
};