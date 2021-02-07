#pragma once

#include "Game.h"

struct VectorChar
{
	double x;
	double y;
};

class Character 
{
private:
	friend class Game;
public:

	Character(float, float);


	int frameToDraw;

	bool jumpKeyUp;	
	bool isHit;

	int startTime;

	int jumpReset;

	VectorChar location;

	VectorChar velocity;

	float health;

	friend class Game;
};