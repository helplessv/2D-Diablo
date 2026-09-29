#include "Chunk.h"
#include <SDL3_image/SDL_image.h>
#include <cmath>

Chunk::Chunk(SDL_Renderer* ren, int index) : renderer(ren){
	const float CHUNK_WIDTH = 1600.0f;
	float startX = index * CHUNK_WIDTH; 
	const float GROUND_Y = 800.0f;

	objects.push_back(std::make_unique<Texture>(renderer, "assets/textures/Ground.png", startX, WINDOW_HEIGHT - 100.0f, 800.0f, 100.0f));
	objects.push_back(std::make_unique<Texture>(renderer, "assets/textures/Ground.png", startX + 800.0f, WINDOW_HEIGHT - 100.0f, 800.0f, 100.0f));

	int amount = rand() % 10;
	float heights[1600];
	for (int i = 0; i < 1600; i++) heights[i] = GROUND_Y;

	for (int i = 0; i < amount; i++) {
		int xPos = rand() % (1600 - 50);
		float currentY = heights[xPos];

		float boxW = 60.0f;
		float boxH = 60.0f;

		float spawnY = currentY - boxH;

		objects.push_back(std::make_unique<Texture>(renderer, "assets/textures/box.png", startX + xPos, spawnY, boxW, boxH));

		for (int j = xPos; j < xPos + 50 && j < 1600; j++) {
			heights[j] = spawnY;
		}
	}
	CameraOffsetX = 0.0f;
}

Chunk::~Chunk() {

}

void Chunk::addBlock(float worldX, float worldY) {
	float blockSize = 60.0f;

	float snappedX = floor(worldX / blockSize) * blockSize;
	float snappedY = floor(worldY / blockSize) * blockSize;

	objects.push_back(std::make_unique<Texture>(
		renderer,
		"assets/textures/box.png",
		snappedX,
		snappedY,
		blockSize,
		blockSize
	));
}

void Chunk::update(float cameraOffsetX) {
	CameraOffsetX = cameraOffsetX;
}

void Chunk::render() {
	for (auto& object : objects) {
		object->draw(CameraOffsetX);
	}
}


std::vector<std::unique_ptr<Texture>>& Chunk::getObjects() {
	return objects;	
}
