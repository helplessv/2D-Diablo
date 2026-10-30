#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include "unit.h"

class Character : public Unit
{
public:
    Character(SDL_Renderer* ren);
    ~Character() override;

    void update(float deltaTime) override;
    void draw(float cameraOffset) override;

    void handleCollisions(const std::vector<SDL_FRect>& platforms);
    float getX();

private:
    struct Animation {
        int frames;
        int animationDelay;
    };

    SDL_FRect& dest;          
    SDL_FRect srcRect;

    SDL_Texture* idleTexture;
    SDL_Texture* TextureLeft;
    SDL_Texture* TextureRight;
    SDL_Texture* currentTexture;

    Animation walking;
    Animation staying;

    int currentFrame;
    Uint64 lastUpdate;
    float velocityY;
    float gravity;
    float jumpPower;
    bool isGrounded;
};