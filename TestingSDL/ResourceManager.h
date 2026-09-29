#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <string>
#include <map>

class ResourceManager {
public:
    static ResourceManager& getInstance();

    SDL_Texture* getTexture(SDL_Renderer* renderer, const std::string& path);

    void clear();

private:
    ResourceManager();
    ~ResourceManager();
    std::map<std::string, SDL_Texture*> textures;
};

#endif
