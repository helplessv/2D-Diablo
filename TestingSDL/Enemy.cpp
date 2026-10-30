#include "Enemy.h"

Enemy::Enemy(SDL_Renderer* ren, const char* texturePath,
    float x, float y, float w, float h,
    int hp, int dmg, float speed,
    float spawnMin, float spawnMax, int drop)
    : Unit(ren, texturePath, x, y, w, h, hp, dmg, speed),
    target(nullptr),
    spawnIntervalMin(spawnMin), spawnIntervalMax(spawnMax),
    drop(drop) {
}

Enemy::~Enemy() {
}

void Enemy::setTarget(const SDL_FRect* t) { target = t; }
float Enemy::getSpawnMin() const { return spawnIntervalMin; }
float Enemy::getSpawnMax() const { return spawnIntervalMax; }
int Enemy::getDrop() const { return drop; }
void Enemy::updateAI(float deltaTime, const std::vector<SDL_FRect>&) {
    update(deltaTime);
}