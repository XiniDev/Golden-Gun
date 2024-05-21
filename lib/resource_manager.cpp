#include "ResourceManager.hpp"

// constructors & destructors

ResourceManager::ResourceManager()
    :textureCache()
{}

ResourceManager::~ResourceManager() {
    // release all resources in the texture cache
    for (auto const& pair : textureCache)
    {
        SDL_DestroyTexture(pair.second);
    }
}

// getters
const std::unordered_map<std::string, SDL_Texture*> ResourceManager::getTextureCache() const
{
    return textureCache;
}

// logic

SDL_Texture* ResourceManager::loadTexture(SDL_Renderer* p_renderer, const std::string& p_texName, const std::string& p_filePath)
{
    // check if texture is already in cache
    if (textureCache.find(p_texName) != textureCache.end())
    {
        std::cout << "Texture " << p_texName << "alreadt exists inside the texture cache." << "\nError: Cannot load pre-existing texture." << std::endl;
        return textureCache[p_texName];
    }

    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(p_renderer, p_filePath.c_str());

    if (texture == NULL)
    {
        std::cout << "Failed to load texture on: " << p_filePath << "\nError: " << IMG_GetError() << std::endl;
        return NULL;
    }

    // cache the texture using the name provided
    textureCache[p_texName] = texture;

    return texture;
}

void ResourceManager::releaseTexture(SDL_Texture* texture) {
    // find texture in cache and remove it
    for (auto const& pair: textureCache)
    {
        if (pair.second == texture)
        {
            SDL_DestroyTexture(texture);
            textureCache.erase(pair.first);
            break;
        }
    }
}