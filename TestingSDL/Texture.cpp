#include "Texture.h"
#include "SDL3/SDL.h"
#include "SDL3_image/SDL_image.h"
#include <string>
#include "ResourceManager.h" 

Texture::Texture(SDL_Renderer* ren, const char* path, float x, float y, float w, float h, bool collision)
    : renderer(ren), posX(x), posY(y), ObjectWidth(w), ObjectHeight(h), stalked(collision) {

    object = ResourceManager::getInstance().getTexture(renderer, path);

    if (object) {
        SDL_SetTextureScaleMode(object, SDL_SCALEMODE_NEAREST);
    }

    dest = { posX, posY, ObjectWidth, ObjectHeight };
}

void Texture::update() {
    dest.x = posX;
    dest.y = posY;
}

void Texture::draw(float cameraOffset) {
    if (object) {
        SDL_FRect renderRect = { dest.x - cameraOffset, dest.y, dest.w, dest.h };
        SDL_RenderTexture(renderer, object, nullptr, &renderRect);
    }
}

SDL_FRect& Texture::getDest() {
    return dest;
}

bool Texture::hasCollision() {
    return stalked;
}

Texture::~Texture() {
    
}