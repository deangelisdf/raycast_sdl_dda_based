#pragma once
#include "SDLWindow.h"
#include "RayCastTileMapManager.h"
class GameEngineSDL : public SDLWindow
{
private:
	double fps;
	bool running; //the game engine is in execution?
	
	RayCastTileMapManager raycastMap;
	SDL_Point pStart;
	SDL_Point pDir;
	SDL_Point pRayCastCollision;
	SDL_FPoint pfRayDir;

	double getTicks();
	void input();
	void update();
	void drawGrid();
	void drawItems();
	void drawCollision();
	void draw();

	GameEngineSDL();
public:
	static GameEngineSDL& getInstance()
	{
		static GameEngineSDL engine;
		return engine;
	}
	~GameEngineSDL();

	void configure();
	void execute();
};

