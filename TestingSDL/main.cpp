#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <random>
#include <string>
#include <map>
#include <cmath>
#include <vector>
#include <memory>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include "Character.h"
#include "Ground.h"
#include "CharAnims.h"
#include "Texture.h"
#include "Chunk.h"
#include "ResourceManager.h"

using namespace std;

const float APPLE_SIZE_X = 20.0f;
const float APPLE_SIZE_Y = 20.0f;
const int WINDOW_HEIGHT = 900;
const int WINDOW_WIDTH = 1600;

int main(int argc, char* argv[]) {
    srand(time(NULL));

    if (!SDL_Init(SDL_INIT_VIDEO) || !TTF_Init()) return 1;

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    if (!SDL_CreateWindowAndRenderer("MyGame", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer)) return 1;
    SDL_SetRenderVSync(renderer, 1);
    //        PLAYER
    Character player(renderer);
    SDL_FRect apple = { 0, 0, 0, 0 };
    TTF_Font* font = TTF_OpenFont("fonts/Bitcount.ttf", 24);
    SDL_Color fontColor = { 150, 0, 50, 255 };
    Uint64 perfFreq = SDL_GetPerformanceFrequency();
    Uint64 lastTime = SDL_GetPerformanceCounter();

    int score = 0;
    int lastScore = -1;
    SDL_Texture* scoreTexture = nullptr;
    bool running = true;
    SDL_Event event;

    float cameraOffset = 0.0f;
    std::map<int, std::unique_ptr<Chunk>> chunks;

    chunks[-1] = std::make_unique<Chunk>(renderer, -1);
    chunks[0] = std::make_unique<Chunk>(renderer, 0);
    chunks[1] = std::make_unique<Chunk>(renderer, 1);

    const float CHUNK_WIDTH = 1600.0f;
    const int CHUNK_RADIUS = 2;

    auto ensureChunk = [&](int index) {
        auto it = chunks.find(index);
        if (it == chunks.end() || !it->second) {
            chunks[index] = std::make_unique<Chunk>(renderer, index);
        }
    };

    lastTime = SDL_GetPerformanceCounter();
    while (running) {
        Uint64 currentTime = SDL_GetPerformanceCounter();
        float deltaTime = (float)(currentTime - lastTime) / (float)perfFreq;
        lastTime = currentTime;
        if (deltaTime > 0.05f) deltaTime = 0.05f;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) running = false;

            if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    float worldX = event.button.x + cameraOffset;
                    float worldY = event.button.y;

                    int chunkIndex = (int)std::floor(worldX / CHUNK_WIDTH);
                    ensureChunk(chunkIndex);
                    chunks[chunkIndex]->addBlock(worldX, worldY);
                }
            }
        }

        int playerChunk = static_cast<int>(std::floor(player.getX() / CHUNK_WIDTH));
        for (int i = playerChunk - CHUNK_RADIUS; i <= playerChunk + CHUNK_RADIUS; ++i) {
            ensureChunk(i);
        }

        player.update(deltaTime);

        cameraOffset = player.getX() - (WINDOW_WIDTH / 2.0f);

        playerChunk = static_cast<int>(std::floor(player.getX() / CHUNK_WIDTH));
        for (int i = playerChunk - CHUNK_RADIUS; i <= playerChunk + CHUNK_RADIUS; ++i) {
            ensureChunk(i);
        }

        std::vector<SDL_FRect> activeWorldObjects;
        for (int i = playerChunk - CHUNK_RADIUS; i <= playerChunk + CHUNK_RADIUS; ++i) {
            auto& chunk = chunks[i];
            chunk->update(cameraOffset);

            for (const auto& obj : chunk->getObjects()) {
                activeWorldObjects.push_back(obj->getDest());
            }
        }

        player.handleCollisions(activeWorldObjects);

        SDL_SetRenderDrawColor(renderer, 70, 70, 255, 255);
        SDL_RenderClear(renderer);

        for (int i = playerChunk - CHUNK_RADIUS; i <= playerChunk + CHUNK_RADIUS; ++i) {
            chunks[i]->render();
        }

        player.draw(cameraOffset);

        SDL_FRect appleDraw = { apple.x - cameraOffset, apple.y, apple.w, apple.h };
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &appleDraw);

        if (score != lastScore) {
            if (scoreTexture) SDL_DestroyTexture(scoreTexture);
            string sText = "Score: " + to_string(score);
            SDL_Surface* surf = TTF_RenderText_Solid(font, sText.c_str(), 0, fontColor);
            if (surf) {
                scoreTexture = SDL_CreateTextureFromSurface(renderer, surf);
                SDL_DestroySurface(surf);
            }
            lastScore = score;
        }
        if (scoreTexture) {
            float tw, th;
            SDL_GetTextureSize(scoreTexture, &tw, &th);
            SDL_FRect dRect = { 40.0f, 20.0f, tw, th };
            SDL_RenderTexture(renderer, scoreTexture, NULL, &dRect);
        }

        SDL_RenderPresent(renderer);
    }

    ResourceManager::getInstance().clear(); 
    if (scoreTexture) { SDL_DestroyTexture(scoreTexture); }
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
