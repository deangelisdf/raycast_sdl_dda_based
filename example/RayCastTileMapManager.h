#pragma once

#include <SDL.h>
#include <unordered_map>

constexpr uint32_t MAX_DIM = 100u;

class RayCastTileMapManager
{
private:
	uint32_t map_width, map_height;
	uint32_t tile_width, tile_height;
	bool matrix[MAX_DIM][MAX_DIM];
public:
	RayCastTileMapManager() : matrix{ false } {}
	void toogleMatrix(uint32_t x, uint32_t y) {matrix[x][y] = !matrix[x][y];}
	bool getItem(uint32_t x, uint32_t y) { return matrix[x][y]; }

	void setMapDimension(uint32_t w, uint32_t h) { this->map_width = w; this->map_height = h; }
	void setTileDimension(uint32_t w, uint32_t h) { this->tile_width = w; this->tile_height = h; }
	
	bool raycast(const SDL_Point& pStart, float fAngle, float fMaxDistance, SDL_FPoint& pFounded);
	bool raycast(const SDL_Point& pStart, const SDL_FPoint& pDir, float fMaxDistance, SDL_FPoint& pFounded);
};

