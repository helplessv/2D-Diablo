#pragma once
#include <SDL3/SDL.h> 
#include <string>
#include "ResourceManager.h"
class Texture
{
public:
    Texture(SDL_Renderer* ren, const char*  path, float x, float y, float w, float h, bool collision);
    ~Texture();

    void update();
    void draw(float cameraOffset);
    SDL_FRect& getDest();
    bool hasCollision();

private:
    SDL_Renderer* renderer;
    SDL_Texture* object;
    SDL_FRect dest;
    float posX, posY;
    float ObjectWidth, ObjectHeight;
    bool stalked;
};
