#include "Character.h"
#include <vector>
#include "ResourceManager.h"

Character::Character(SDL_Renderer* ren) : renderer(ren) {
    Texture = ResourceManager::getInstance().getTexture(renderer, "assets/animations/MainCharStaying.png");
    TextureLeft = ResourceManager::getInstance().getTexture(renderer, "assets/animations/walkingleft.png");
    TextureRight = ResourceManager::getInstance().getTexture(renderer, "assets/animations/walkingright.png");
    if (!Texture) {
        SDL_Log("Texture Error: %s", SDL_GetError());
    }
    SDL_SetTextureScaleMode(Texture, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(TextureLeft, SDL_SCALEMODE_NEAREST);
    SDL_SetTextureScaleMode(TextureRight, SDL_SCALEMODE_NEAREST);

    staying.frames = 2;
    staying.animationDelay = 400;

    walking.frames = 8; 
    walking.animationDelay = 80;

    currentFrame = 0;
    lastUpdate = SDL_GetTicks();
    currentTexture = Texture;

    srcRect = { 0.0f, 0.0f, 18.0f, 44.0f };
    dest = { 380.0f, 275.0f, 40.0f, 100.0f };
    speed = 500.0f;
    gravity = 2500.0f;
    jumpPower = 1000.0f;
    velocityY = 0.0f;
    isGrounded = false;
    posX = 0.0f;
}

Character::~Character() {
}

void Character::handleCollisions(const std::vector<SDL_FRect>& platforms) {
    isGrounded = false;

    for (const auto& platform : platforms) {
        if (!SDL_HasRectIntersectionFloat(&dest, &platform)) continue;

        float overlapX = (dest.x + dest.w / 2 < platform.x + platform.w / 2)
            ? (dest.x + dest.w - platform.x)
            : (platform.x + platform.w - dest.x);
        float overlapY = (dest.y + dest.h / 2 < platform.y + platform.h / 2)
            ? (dest.y + dest.h - platform.y)
            : (platform.y + platform.h - dest.y);

        if (overlapX < overlapY) {
            if (dest.x + dest.w / 2 < platform.x + platform.w / 2)
                dest.x -= overlapX;
            else
                dest.x += overlapX;
        }
        else {
            if (dest.y + dest.h / 2 < platform.y + platform.h / 2) {
                dest.y -= overlapY;
                velocityY = 0;
                isGrounded = true;
            }
            else {
                dest.y += overlapY;
                velocityY = 0;
            }
        }
    }
}
void Character::update(float deltaTime) {
    const bool* keys = SDL_GetKeyboardState(NULL);
    bool moving = false;
    SDL_Texture* lastTexture = currentTexture;

    if ((keys[SDL_SCANCODE_W] || keys[SDL_SCANCODE_SPACE]) && isGrounded) {
        velocityY = -jumpPower;
        isGrounded = false;
    }

    if (keys[SDL_SCANCODE_A]) {
        dest.x -= speed * deltaTime;
        currentTexture = TextureLeft;
        moving = true;
    }
    else if (keys[SDL_SCANCODE_D]) {
        dest.x += speed * deltaTime;
        currentTexture = TextureRight;
        moving = true;
    }
    else {
        currentTexture = Texture;
    }

    if (currentTexture != lastTexture) {
        currentFrame = 0;
        lastUpdate = SDL_GetTicks();
    }

    float texW, texH;
    SDL_GetTextureSize(currentTexture, &texW, &texH);
    const Animation* currentAnim = moving ? &walking : &staying;

    float frameWidth = texW / currentAnim->frames;
    srcRect.w = frameWidth;
    srcRect.h = texH;

    velocityY += gravity * deltaTime;
    dest.y += velocityY * deltaTime;

    if (dest.y >= 900 - dest.h) {
        dest.y = 900 - dest.h;
        velocityY = 0;
        isGrounded = true;
    }

    Uint64 now = SDL_GetTicks();
    if (now - lastUpdate >= currentAnim->animationDelay) {
        lastUpdate = now;
        currentFrame = (currentFrame + 1) % currentAnim->frames;
    }

    srcRect.x = (float)currentFrame * srcRect.w;
}




void Character::draw(float cameraOffset) {
    if (currentTexture) {
        SDL_FRect renderPos = dest;
        renderPos.x -= cameraOffset;
        SDL_RenderTexture(renderer, currentTexture, &srcRect, &renderPos);
    }
}

float Character::getX() { return dest.x; }
SDL_FRect* Character::getDest() { return &dest; }
