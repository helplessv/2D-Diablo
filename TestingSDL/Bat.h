#pragma once
#include "Enemy.h"

class Bat : public Enemy
{
public:
    static constexpr float SPAWN_MIN = 7.0f;
    static constexpr float SPAWN_MAX = 11.0f;

    Bat(SDL_Renderer* ren, float x, float y);
    ~Bat() override;

    void updateAI(float deltaTime, const std::vector<SDL_FRect>& obstacles) override;
    void update(float deltaTime) override;  
    void draw(float cameraOffset) override;

private:
    SDL_FRect hitboxAt(float x, float y);
    bool isDirFree(float dirX, float dirY, float maxDist, const std::vector<SDL_FRect>& obstacles);
    void resolveCollisions(const std::vector<SDL_FRect>& obstacles);

    SDL_Texture* sheet;
    SDL_FRect srcRect;
    int frames;
    int currentFrame;
    int frameDelay;   
    Uint64 lastUpdate;
    bool facingRight;

    float vx, vy;        
    float avoidSide;    
};