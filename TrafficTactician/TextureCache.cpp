#include "TextureCache.h"
#include "Texture.h"

std::map<std::string, Texture*> TextureCache::cache;

Texture* TextureCache::loadTexture(const std::string& fileName)
{
    auto found = cache.find(fileName);
    if (found != cache.end())
        return found->second;
    auto texture = new Texture(fileName);
    cache[fileName] = texture;
    return texture;
}

Texture* TextureCache::loadMaterialTexture(const std::string& colorName, const glm::vec3& color)
{
    const std::string materialName = colorName;
    auto found = cache.find(materialName);
    if (found != cache.end())
        return found->second;
    auto texture = new Texture(color);
    cache[colorName] = texture;
    return texture;
}
