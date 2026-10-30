#include "Bat.h"
#include "ResourceManager.h"
#include <cmath>
#include <algorithm>

namespace {
    const float PI = 3.14159265f;
    const float ANGLE_STEP = 30.0f * PI / 180.0f;
    const float HITBOX_INSET_X = 16.0f;  
    const float HITBOX_INSET_Y = 10.0f;
    const float LOOKAHEAD = 90.0f;       
    const float PROBE_STEP = 15.0f;      
    const float TURN_RATE = 8.0f;        

    bool anyHit(const SDL_FRect& r, const std::vector<SDL_FRect>& obstacles) {
        for (const auto& o : obstacles) {
            if (SDL_HasRectIntersectionFloat(&r, &o)) return true;
        }
        return false;
    }
}

Bat::Bat(SDL_Renderer* ren, float x, float y)
    : Enemy(ren, "assets/textures/bat.png",
        x, y, 112.0f, 60.0f,
        20, 5, 200.0f,
        SPAWN_MIN, SPAWN_MAX, 1),
    frames(5), currentFrame(0), frameDelay(100),
    lastUpdate(SDL_GetTicks()), facingRight(false),
    vx(0.0f), vy(0.0f), avoidSide(1.0f)
{
    sheet = ResourceManager::getInstance().getTexture(renderer, "assets/textures/bat.png");
    if (sheet) {
        SDL_SetTextureScaleMode(sheet, SDL_SCALEMODE_NEAREST);
    }
    srcRect = { 0.0f, 0.0f, 56.0f, 30.0f };
}

Bat::~Bat() {
}

SDL_FRect Bat::hitboxAt(float x, float y) {
    SDL_FRect& d = getDest();
    return { x + HITBOX_INSET_X, y + HITBOX_INSET_Y,
             d.w - 2.0f * HITBOX_INSET_X, d.h - 2.0f * HITBOX_INSET_Y };
}

bool Bat::isDirFree(float dirX, float dirY, float maxDist,
    const std::vector<SDL_FRect>& obstacles) {
    SDL_FRect& d = getDest();
    for (float s = PROBE_STEP; s <= maxDist + 0.01f; s += PROBE_STEP) {
        SDL_FRect hb = hitboxAt(d.x + dirX * s, d.y + dirY * s);
        if (anyHit(hb, obstacles)) return false;
    }
    return true;
}

void Bat::resolveCollisions(const std::vector<SDL_FRect>& obstacles) {
    SDL_FRect& d = getDest();
    for (const auto& o : obstacles) {
        SDL_FRect hb = hitboxAt(d.x, d.y);
        if (!SDL_HasRectIntersectionFloat(&hb, &o)) continue;

        bool fromLeft = hb.x + hb.w / 2 < o.x + o.w / 2;
        bool fromTop = hb.y + hb.h / 2 < o.y + o.h / 2;
        float overlapX = fromLeft ? (hb.x + hb.w - o.x) : (o.x + o.w - hb.x);
        float overlapY = fromTop ? (hb.y + hb.h - o.y) : (o.y + o.h - hb.y);

        if (overlapX < overlapY) {
            d.x += fromLeft ? -overlapX : overlapX;
            vx = 0.0f;
        }
        else {
            d.y += fromTop ? -overlapY : overlapY;
            vy = 0.0f;
        }
    }
}

void Bat::updateAI(float deltaTime, const std::vector<SDL_FRect>& obstacles) {
    SDL_FRect& d = getDest();
    float desiredVx = 0.0f, desiredVy = 0.0f;

    if (target) {
        float cx = d.x + d.w / 2.0f;
        float cy = d.y + d.h / 2.0f;
        float dx = (target->x + target->w / 2.0f) - cx;
        float dy = (target->y + target->h / 2.0f) - cy;
        float dist = std::sqrt(dx * dx + dy * dy);

        if (dist > 1.0f) {
            float dirX = dx / dist;
            float dirY = dy / dist;
            float look = std::max(PROBE_STEP, std::min(LOOKAHEAD, dist));

            if (!isDirFree(dirX, dirY, look, obstacles)) {
                float base = std::atan2(dy, dx);
                bool found = false;
                for (int k = 1; k <= 6 && !found; ++k) {
                    for (int s = 0; s < 2 && !found; ++s) {
                        float side = (s == 0) ? avoidSide : -avoidSide;
                        float a = base + side * k * ANGLE_STEP;
                        float ax = std::cos(a), ay = std::sin(a);
                        if (isDirFree(ax, ay, look, obstacles)) {
                            dirX = ax;
                            dirY = ay;
                            avoidSide = side;
                            found = true;
                        }
                    }
                }
            }

            desiredVx = dirX * speed;
            desiredVy = dirY * speed;
        }
    }

    float t = std::min(1.0f, TURN_RATE * deltaTime);
    vx += (desiredVx - vx) * t;
    vy += (desiredVy - vy) * t;

    d.x += vx * deltaTime;
    d.y += vy * deltaTime;

    resolveCollisions(obstacles);

    if (std::fabs(vx) > 5.0f) facingRight = vx > 0.0f;

    update(deltaTime);
}

void Bat::update(float deltaTime) {
    (void)deltaTime;
    if (sheet) {
        float texW, texH;
        SDL_GetTextureSize(sheet, &texW, &texH);
        srcRect.w = texW / frames;
        srcRect.h = texH;
    }

    Uint64 now = SDL_GetTicks();
    if (now - lastUpdate >= (Uint64)frameDelay) {
        lastUpdate = now;
        currentFrame = (currentFrame + 1) % frames;
    }
    srcRect.x = (float)currentFrame * srcRect.w;
}

void Bat::draw(float cameraOffset) {
    if (!sheet) return;
    SDL_FRect pos = getDest();
    pos.x -= cameraOffset;
    SDL_RenderTextureRotated(renderer, sheet, &srcRect, &pos, 0.0, nullptr,
        facingRight ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}