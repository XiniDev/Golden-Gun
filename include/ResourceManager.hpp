#pragma once

#include <iostream>
#include <utility>
#include <string>
#include <unordered_map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class ResourceManager
{
    private:
        std::unordered_map<std::string, SDL_Texture*> textureCache;

    public:
        // constructors & destructors
        ResourceManager();
        ~ResourceManager();

        // getters
        const std::unordered_map<std::string, SDL_Texture*> getTextureCache() const;

        // logic
        SDL_Texture* loadTexture(SDL_Renderer* p_renderer, const std::string& p_texName, const std::string& p_filePath);
        void releaseTexture(SDL_Texture* p_texture);
};