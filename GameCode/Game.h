#pragma once
#include "Characters.h" 
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>


struct VectorGame
{
	float x;
	float y;
};

class Game 
{
public:
	Game();
	~Game();
	bool Initialize();
	bool loadFromFile(std::string);
	void free();
	void RunLoop();
	void close();
	int getWidth();
	int getHeight();
	void render(int , int , SDL_Rect* clip);
	bool loadMediaFirst();
	void getFrameRight(int, int, int);
	void getFrameLeft(int, int, int);
	void getFrameUpAndDown(int, int, int);
	bool checkCollision(SDL_Rect);
	void objects();
	void displayFrame(Character);
	friend class Character;

	bool hit(SDL_Rect, SDL_Rect);
	
private:

	void ProcessInput();
	void GenerateOutput();

	Character UpdatePlayer(Character,SDL_Rect);

	SDL_Texture* mTexture;
	
	Uint32 mTicksCount;
	
	int mWidth;
	int mHeight;
	
	bool mIsRunning;

	friend class Character;

};

