#pragma once
#include "unit.h"
#include <vector>

class Enemy : public Unit
{
public:
    Enemy(SDL_Renderer* ren, const char* texturePath,
        float x, float y, float w, float h,
        int hp, int dmg, float speed,
        float spawnMin, float spawnMax, int drop);
    ~Enemy() override;

    void setTarget(const SDL_FRect* t);   

    float getSpawnMin() const;
    float getSpawnMax() const;
    int getDrop() const;
    virtual void updateAI(float deltaTime, const std::vector<SDL_FRect>& obstacles);

protected:
    const SDL_FRect* target;
    float spawnIntervalMin;
    float spawnIntervalMax;
    int drop;
};