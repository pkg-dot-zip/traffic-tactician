#pragma once

#include <map>
#include <string>
#include <glm/fwd.hpp>

class Texture;

class TextureCache
{
	static std::map<std::string, Texture*> cache;

public:
	static Texture* loadTexture(const std::string& fileName);
	static Texture* loadMaterialTexture(const std::string& fileName, const glm::vec3& color);
};
