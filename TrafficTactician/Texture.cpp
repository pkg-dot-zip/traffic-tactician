#include "Texture.h"
#include <glm/vec3.hpp>
#define STB_IMAGE_IMPLEMENTATION

#include "easylogging++.h"
#include "stb_image.h"

Texture::Texture(const std::string& fileName)
{
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	int w, h, comp;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(fileName.c_str(), &w, &h, &comp, 4);
	if (!data)
	{
		LOG(INFO) << fileName.c_str() << " -> " << stbi_failure_reason() << std::endl;
		exit(0);
	}

	LOG(INFO) << "Loaded texture " << fileName.c_str() << std::endl;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	stbi_image_free(data);
}

Texture::Texture(const glm::vec3& color)
{
	int size = 1;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	// Create image data for 1x1 with given color
	unsigned char* data = new unsigned char[3 * size * size * sizeof(unsigned char)];
	for (unsigned int i = 0; i < size * size; i++)
	{
		data[i * 3] = (unsigned char)(color.x * 255.0f);
		data[i * 3 + 1] = (unsigned char)(color.y * 255.0f);
		data[i * 3 + 2] = (unsigned char)(color.z * 255.0f);
	}
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	delete[] data;
}

Texture::~Texture() {
	unbind();
	glDeleteTextures(1, &id);
}

void Texture::bind()
{
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}