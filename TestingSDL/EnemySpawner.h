#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include <memory>
#include <cstdlib>
#include <algorithm>
#include "Enemy.h"
#include "Bat.h"

class EnemySpawner
{
public:
    EnemySpawner(SDL_Renderer* ren) : renderer(ren), gameTime(0.0f) {
        batTimer = nextBatInterval();
    }

    void update(float deltaTime, float cameraOffset, const SDL_FRect* target,
        const std::vector<SDL_FRect>& obstacles) {
        gameTime += deltaTime;

        batTimer -= deltaTime;
        if (batTimer <= 0.0f) {
            spawnBat(cameraOffset, target);
            batTimer = nextBatInterval();
        }

        for (auto& e : enemies) e->updateAI(deltaTime, obstacles);

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }),
            enemies.end());
    }

    void draw(float cameraOffset) {
        for (auto& e : enemies) e->draw(cameraOffset);
    }

    std::vector<std::unique_ptr<Enemy>>& getEnemies() { return enemies; }

private:
    float spawnRate(float t) const {
        (void)t;
        return 1.0f;
    }

    float nextBatInterval() const {
        float r = (float)rand() / (float)RAND_MAX;
        float interval = Bat::SPAWN_MIN + r * (Bat::SPAWN_MAX - Bat::SPAWN_MIN);
        return interval / spawnRate(gameTime);
    }

    void spawnBat(float cameraOffset, const SDL_FRect* target) {
        const float SCREEN_W = 1600.0f;
        bool left = (rand() % 2) == 0;
        float x = left ? cameraOffset - 150.0f : cameraOffset + SCREEN_W + 50.0f;
        float y = 50.0f + (float)(rand() % 350);

        auto bat = std::make_unique<Bat>(renderer, x, y);
        bat->setTarget(target);
        enemies.push_back(std::move(bat));
    }

    SDL_Renderer* renderer;
    std::vector<std::unique_ptr<Enemy>> enemies;
    float gameTime;
    float batTimer;
};