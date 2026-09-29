#pragma once
#include <SDL3/SDL.h>
#include "Texture.h"
#include <vector>
#include <SDL3_image/SDL_image.h>
#include <memory>

class Chunk
{
public:
	Chunk(SDL_Renderer* ren, int index);;
	~Chunk();
	void update(float cameraOffsetX);
	std::vector<std::unique_ptr<Texture>>& getObjects();
	void render();
	void addBlock(float worldX, float worldY);

private:
	const float WINDOW_HEIGHT = 900.0f;
	const float WINDOW_WIDTH = 1600.0f;
	SDL_Renderer* renderer;
	std::string position;
	std::vector<std::unique_ptr<Texture>> objects;
	float CameraOffsetX;
	float x;
};

