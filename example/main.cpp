#include "GameEngineSDL.h"

int main(int argc, char* argv[])
{
	GameEngineSDL& game = GameEngineSDL::getInstance();
	game.configure();
	game.execute();
	return 0;
}