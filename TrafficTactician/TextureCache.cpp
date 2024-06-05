#include "TextureCache.h"
#include "Texture.h"

enum Pose { POSE_MOVE_RIGHT, POSE_MOVE_LEFT, POSE_MOVE_FORWARD, POSE_STOP, POSE_OTHER };

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
