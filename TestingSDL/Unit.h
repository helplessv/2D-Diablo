#pragma once
#include <SDL3/SDL.h>
#include "Texture.h"

class Unit
{
public:
    Unit(SDL_Renderer* ren, const char* texturePath,
        float x, float y, float w, float h,
        int hp, int dmg, float speed);
    virtual ~Unit();

    virtual void update(float deltaTime);
    virtual void draw(float cameraOffset);

    void moveX(float direction, float deltaTime);
    void takeDamage(int amount);
    void heal(int amount);
    void addExp(int amount);
    bool isAlive() const;

    int getHp() const;
    int getMaxHp() const;
    int getExp() const;
    int getDmg() const;
    float getSpeed() const;
    SDL_FRect& getDest();

protected:
    SDL_Renderer* renderer;
    Texture texture;
    int hp;
    int maxHp;
    int exp;
    int dmg;
    float speed;
};