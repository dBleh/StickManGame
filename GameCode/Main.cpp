#include "Game.h"

int main(int argc, char** argv)
{
	
	Game game;
	bool success = game.Initialize();
	if (success)
	{
		game.RunLoop();
	}
	game.close();
	return 0;
}