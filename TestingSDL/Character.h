#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#pragma once

typedef struct animation {
    int frames;
    int y;
    int animationDelay;
};

class Character
{
public:
    Character(SDL_Renderer* ren);

    ~Character();

    void update(float deltaTime);

    void draw(float cameraOffset);

    SDL_FRect* getDest();

    void handleCollisions(const std::vector<SDL_FRect>& platforms);

    float getX();
private:
    SDL_Renderer* renderer;
    SDL_FRect srcRect;
    SDL_FRect dest;
    SDL_Texture* Texture;
    SDL_Texture* TextureLeft;
    SDL_Texture* TextureRight;
    SDL_Texture* currentTexture;

    struct Animation {
        int frames;
        int animationDelay;
        float width;
    };

    Animation walking;
    Animation staying;

    float speed;
    int currentFrame;
    Uint64 lastUpdate;
    float velocityY;
    float gravity;    
    float jumpPower;   
    bool isGrounded;   
    float posX;
    float srcWidth;
};
