#include "unit.h"
#include <algorithm>

Unit::Unit(SDL_Renderer* ren, const char* texturePath,
    float x, float y, float w, float h,
    int hp, int dmg, float speed)
    : renderer(ren),
    texture(ren, texturePath, x, y, w, h, false),
    hp(hp), maxHp(hp), exp(0), dmg(dmg), speed(speed) {
}

Unit::~Unit() {
}

void Unit::update(float deltaTime) {
}

void Unit::draw(float cameraOffset) {
    texture.draw(cameraOffset);
}

void Unit::moveX(float direction, float deltaTime) {
    texture.getDest().x += direction * speed * deltaTime;
}

void Unit::takeDamage(int amount) {
    hp = std::max(0, hp - amount);
}

void Unit::heal(int amount) {
    hp = std::min(maxHp, hp + amount);
}

void Unit::addExp(int amount) {
    exp += amount;
}

bool Unit::isAlive() const { return hp > 0; }

int Unit::getHp() const { return hp; }
int Unit::getMaxHp() const { return maxHp; }
int Unit::getExp() const { return exp; }
int Unit::getDmg() const { return dmg; }
float Unit::getSpeed() const { return speed; }
SDL_FRect& Unit::getDest() { return texture.getDest(); }