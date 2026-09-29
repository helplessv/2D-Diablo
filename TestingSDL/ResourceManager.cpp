#include "ResourceManager.h"

ResourceManager::ResourceManager() {}

ResourceManager::~ResourceManager() { clear(); }

void ResourceManager::clear() {
    for (auto& pair : textures) {
        SDL_DestroyTexture(pair.second);
    }
    textures.clear();
}

SDL_Texture* ResourceManager::getTexture(SDL_Renderer* renderer, const std::string& path) {
    if (textures.find(path) != textures.end()) {
        return textures[path];
    }

    SDL_Texture* newTexture = IMG_LoadTexture(renderer, path.c_str());
    if (!newTexture) {
        SDL_Log("Ошибка загрузки: %s", SDL_GetError());
        return nullptr;
    }

    textures[path] = newTexture;
    return newTexture;
}

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}