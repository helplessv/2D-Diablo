#pragma once
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

class Ground
{
public:
	Ground(SDL_Renderer* ren);
	~Ground();
	void draw();
	SDL_FRect& getDest();
	void update(float move);
private:
	SDL_Renderer* renderer;
	SDL_Texture* Texture;
	SDL_FRect dest;
	SDL_FRect srcRect;
	float groundWidth;
	float groundHeight;
	float posX;
	float move;
};
