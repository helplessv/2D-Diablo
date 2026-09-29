#pragma once
#include <SDL3/SDL.h> 
#include <string>
#include "ResourceManager.h"
class Texture
{
public:
    Texture(SDL_Renderer* ren, const char*  path, float x, float y, float w, float h);
    ~Texture();

    void update();
    void draw(float cameraOffset);
    SDL_FRect& getDest();

private:
    SDL_Renderer* renderer;
    SDL_Texture* object;
    SDL_FRect dest;
    float posX, posY;
    float ObjectWidth, ObjectHeight;
};
