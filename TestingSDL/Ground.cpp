#include "Ground.h"

Ground::Ground(SDL_Renderer* ren) : renderer(ren){
	Texture = IMG_LoadTexture(renderer, "seprites/Ground.png");
    if (!Texture) {
        SDL_Log("Texture Error: %s", SDL_GetError());
    }
    SDL_SetTextureScaleMode(Texture, SDL_SCALEMODE_NEAREST);

    groundWidth = 800.0f;
    groundHeight = 100.0f;
    posX = 0.0f;
    move = 0;
    dest = { posX, 500.0f, groundWidth, groundHeight };
}

void Ground::update(float move) {
    dest.x = posX - move;
}

void Ground::draw() {
    if (Texture) {
        SDL_RenderTexture(renderer, Texture, nullptr, &dest);
    }
}

SDL_FRect& Ground::getDest() {
    return dest;
}

Ground::~Ground() {
    if (Texture) SDL_DestroyTexture(Texture);
}


