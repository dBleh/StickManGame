#include "Game.h"
#include "Characters.h" 



const int thickness = 15;
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;

const int xSpeed = 12;

const int jumpVelocity = -13;
const double gravity = .41;

SDL_Rect gRunningRight[4];
SDL_Rect gRunningLeft[4];
SDL_Rect gUpAndDown[2];
SDL_Rect wall[10];
SDL_Rect* result = new SDL_Rect();
SDL_Rect playerOneAttackHitbox[3];
SDL_Rect playerTwoAttackHitbox[3];


Game gSpriteSheetTexture;

SDL_Surface* sBackground = NULL;
SDL_Texture* tBackground = NULL;

SDL_Window* gWindow = NULL;

SDL_Renderer* gRenderer = NULL;

Character playerOne(0, 800);
SDL_Rect playerOneHitBox;
Character playerTwo(0, 0);
SDL_Rect playerTwoHitBox;

Game::Game()
{
	
	gWindow=nullptr;
	gRenderer=nullptr;
	mIsRunning=true;

	mTexture = NULL;

	mWidth = 0;
	mHeight = 0;

	mTicksCount = 0;

}
Game::~Game()
{
	free();
}
Character::Character(float x, float y)//pass the start location.
{
	
	frameToDraw = 0;
	
	location.x = x;
	location.y = y;

	startTime = SDL_GetTicks();

	jumpReset = 1;

	velocity.y = 0;
	velocity.x = 0;

	jumpKeyUp = true;

	health = 0;

}

bool Game::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		Uint32 colorKey = SDL_MapRGB(loadedSurface->format, 255, 255, 255);
		SDL_SetColorKey(loadedSurface, SDL_TRUE, colorKey);
		newTexture = SDL_CreateTextureFromSurface(gRenderer,loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{		
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}	
		SDL_FreeSurface(loadedSurface);
	}

	mTexture = newTexture;
	return mTexture != NULL;
}

void Game::free()
{
	
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void Game::render(int x, int y, SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
}

int Game::getWidth()
{
	return mWidth;
}
int Game::getHeight()
{
	return mHeight;
}

bool Game::Initialize()
{
	
	bool success = true;
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			sBackground = SDL_LoadBMP("BackGround.bmp");
			tBackground = SDL_CreateTextureFromSurface(gRenderer, sBackground);
			
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor(gRenderer, 255, 0, 255, 0xFF);

				
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

void Game::close()
{
	gSpriteSheetTexture.free();
	
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	SDL_DestroyTexture(tBackground);
	
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

void Game::RunLoop()
{
	while (mIsRunning)
	{
		ProcessInput();

		playerOne = UpdatePlayer(playerOne, playerOneHitBox);
		playerTwo = UpdatePlayer(playerTwo, playerTwoHitBox);
		
		objects();
		GenerateOutput();

	}
}

bool Game::loadMediaFirst()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if (!gSpriteSheetTexture.loadFromFile("BaseTemplate.png"))
	{
		printf("Failed to load sprite sheet texture!\n");
		success = false;
	}
	else
	{
		/*gRunningLeft[0].x = 185;
		gRunningLeft[0].y = 178;
		gRunningLeft[0].w = 58;
		gRunningLeft[0].h = 123;

		gRunningLeft[1].x = 330;
		gRunningLeft[1].y = 178;
		gRunningLeft[1].w = 58;
		gRunningLeft[1].h = 123;

		gRunningLeft[2].x = 475;
		gRunningLeft[2].y = 178;
		gRunningLeft[2].w = 58;
		gRunningLeft[2].h = 123;

		gRunningLeft[3].x = 628;
		gRunningLeft[3].y = 178;
		gRunningLeft[3].w = 58;
		gRunningLeft[3].h = 123;*/

		gRunningLeft[0].x = 185;
		gRunningLeft[0].y = 519;
		gRunningLeft[0].w = 58;
		gRunningLeft[0].h = 123;

		gRunningLeft[1].x = 330;
		gRunningLeft[1].y = 519;
		gRunningLeft[1].w = 58;
		gRunningLeft[1].h = 123;

		gRunningLeft[2].x = 475;
		gRunningLeft[2].y = 519;
		gRunningLeft[2].w = 58;
		gRunningLeft[2].h = 123;

		gRunningLeft[3].x = 628;
		gRunningLeft[3].y = 519;
		gRunningLeft[3].w = 58;
		gRunningLeft[3].h = 123;

		
		/*gRunningRight[0].x = 185;
		gRunningRight[0].y = 12;
		gRunningRight[0].w = 58;
		gRunningRight[0].h = 123;

		gRunningRight[1].x = 341;
		gRunningRight[1].y = 12;
		gRunningRight[1].w = 58;
		gRunningRight[1].h = 123;

		gRunningRight[2].x = 488;
		gRunningRight[2].y = 12;
		gRunningRight[2].w = 58;
		gRunningRight[2].h = 123;

		gRunningRight[3].x = 626;
		gRunningRight[3].y = 12;
		gRunningRight[3].w = 58;
		gRunningRight[3].h = 123;*/

		gRunningRight[0].x = 185;
		gRunningRight[0].y = 338;
		gRunningRight[0].w = 58;
		gRunningRight[0].h = 123;

		gRunningRight[1].x = 341;
		gRunningRight[1].y = 338;
		gRunningRight[1].w = 58;
		gRunningRight[1].h = 123;

		gRunningRight[2].x = 488;
		gRunningRight[2].y = 338;
		gRunningRight[2].w = 58;
		gRunningRight[2].h = 123;

		gRunningRight[3].x = 626;
		gRunningRight[3].y = 338;
		gRunningRight[3].w = 58;
		gRunningRight[3].h = 123;

		gUpAndDown[0].x = 47;
		gUpAndDown[0].y = 12;
		gUpAndDown[0].w = 58;
		gUpAndDown[0].h = 123;

		gUpAndDown[1].x = 43;
		gUpAndDown[1].y = 178;
		gUpAndDown[1].w = 58;
		gUpAndDown[1].h = 123;
	
	}
	return success;
}

void Game::getFrameRight(int frame, int locationx, int locationy)
{
	if (!loadMediaFirst())
	{
		printf("Failed to load second media!\n");
	}
	if (frame == 0)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningRight[0]);
	}
	if (frame == 1)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningRight[1]);
	}
	if (frame == 2)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningRight[2]);
	}
	if (frame == 3)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningRight[3]);
	}
}
void Game::getFrameLeft(int frame, int locationx, int locationy)
{
	if (!loadMediaFirst())
	{
		printf("Failed to load second media!\n");
	}
	if (frame == 0)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningLeft[0]);
	}
	if (frame == 1)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningLeft[1]);
	}
	if (frame == 2)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningLeft[2]);
	}
	if (frame == 3)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gRunningLeft[3]);
	}

}
void Game::getFrameUpAndDown(int frame, int locationx, int locationy) {
	if (!loadMediaFirst())
	{
		printf("Failed to load second media!\n");
	}
	if (frame == 0)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gUpAndDown[0]);
	}
	if (frame == 1)
	{
		gSpriteSheetTexture.render(locationx, locationy, &gUpAndDown[1]);
	}
}

bool Game::checkCollision(SDL_Rect c)
{
	if (SDL_IntersectRect(&wall[3], &c, result) || SDL_IntersectRect(&wall[0], &c, result) || SDL_IntersectRect(&wall[1], &c, result) || SDL_IntersectRect(&wall[2], &c, result))// add a larger hit registration check, rather than checking for 1 pixel check to see if it till hit 3 pi		
		return true;
	return false;
}

bool Game::hit(SDL_Rect attacker, SDL_Rect attackeee)
{

	if (SDL_IntersectRect(&attacker, &attackeee, result)) {
		std::cout << "its happening" << std::endl;
		return true;
	}
	return false;
}

void Game::objects()
{
	
	wall[0].x = -400;
	wall[0].y = SCREEN_HEIGHT - 10 ;
	wall[0].h = 20;
	wall[0].w = 3000;

	wall[3].x = ((SCREEN_WIDTH / 2) - 400);
	wall[3].y = 700 ;
	wall[3].h = 20;
	wall[3].w = 800;

	playerOneAttackHitbox[0].x = 58 + playerOne.location.x;
	playerOneAttackHitbox[0].y = 31 + playerOne.location.y;
	playerOneAttackHitbox[0].w = 29;
	playerOneAttackHitbox[0].h = 61;

	playerOneAttackHitbox[1].x = -48 + playerOne.location.x;
	playerOneAttackHitbox[1].y = 31 + playerOne.location.y;
	playerOneAttackHitbox[1].w = 29;
	playerOneAttackHitbox[1].h = 61;

	playerTwoAttackHitbox[0].x = 58 + playerTwo.location.x;
	playerTwoAttackHitbox[0].y = 31 + playerTwo.location.y;
	playerTwoAttackHitbox[0].w = 29;
	playerTwoAttackHitbox[0].h = 61;

	playerTwoAttackHitbox[1].x = -48 + playerTwo.location.x;
	playerTwoAttackHitbox[1].y = 31 + playerTwo.location.y;
	playerTwoAttackHitbox[1].w = 29;
	playerTwoAttackHitbox[1].h = 61;

	playerOneHitBox.y = playerOne.location.y;
	playerOneHitBox.y = playerOne.location.y;
	playerOneHitBox.w = 58;
	playerOneHitBox.h = 123;

	playerTwoHitBox.y = playerTwo.location.y;
	playerTwoHitBox.x = playerTwo.location.x;
	playerTwoHitBox.w = 58;
	playerTwoHitBox.h = 123;
}

void Game::ProcessInput()
{
	int mouseX;
	int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	const Uint8* state = SDL_GetKeyboardState(NULL);

	SDL_Event e;
	
	while (SDL_PollEvent(&e) != 0)
	{
		
		switch (e.type)
		{
		case SDL_QUIT:
			mIsRunning = false;
			break;
		case SDL_MOUSEBUTTONDOWN: 
			if (e.button.button == SDL_BUTTON_LEFT)
			{
				if (hit(playerOneAttackHitbox[1], playerTwoHitBox))
				{
					playerTwo.health += 20;
					
					playerTwo.velocity.x -= (playerTwo.health );
					playerTwo.isHit = true;
				}
			}
			
			if (e.button.button == SDL_BUTTON_RIGHT)
			{			
				if (hit(playerOneAttackHitbox[0], playerTwoHitBox))
				{	
					playerTwo.health += 20;
					playerTwo.velocity.x += (playerTwo.health );
					playerTwo.isHit = true;
				}
			}
			break;
		case SDL_KEYUP:
			playerOne.jumpKeyUp = true;	
			playerTwo.jumpKeyUp = true;
			break;
		
		
		}
	}
	playerOne.velocity.x = 0;
	playerTwo.velocity.x = 0;

	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
	
	if (state[SDL_SCANCODE_D])
	{
		if (state[SDL_SCANCODE_LSHIFT]) 
		{	
			playerOne.velocity.x = xSpeed;
		}
		else 
		{
			playerOne.velocity.x = xSpeed;

		}
	}
	if (state[SDL_SCANCODE_A])
	{
		if (state[SDL_SCANCODE_LSHIFT])
		{
			playerOne.velocity.x = -xSpeed;
		}
		else
		{
			playerOne.velocity.x = -xSpeed;
		}

	}
	if (state[SDL_SCANCODE_W])
	{	
		
	}
	if (state[SDL_SCANCODE_S])
	{
		playerOne.velocity.y = 9;
	}
	if (state[SDL_SCANCODE_SPACE] && playerOne.jumpReset > 0)
	{
		if (playerOne.jumpReset == 3 && playerOne.jumpKeyUp)
		{
			
			playerOne.startTime = (SDL_GetTicks() / 50);
			playerOne.jumpReset = playerOne.jumpReset - 1;
			playerOne.velocity.y = jumpVelocity;
			playerOne.jumpKeyUp = false;
		}

		if (playerOne.jumpReset == 2 && playerOne.jumpKeyUp)
		{
			if (playerOne.startTime + 5 < SDL_GetTicks() / 50) {

				playerOne.startTime = (SDL_GetTicks() / 50);
				playerOne.jumpReset = playerOne.jumpReset - 1;
				playerOne.velocity.y = jumpVelocity;
				playerOne.jumpKeyUp = false;
			}
		}
		if (playerOne.jumpReset == 1 && playerOne.jumpKeyUp)
		{
			if (playerOne.startTime + 5 < SDL_GetTicks() / 50) {
				playerOne.jumpReset = playerOne.jumpReset - 1;
				playerOne.velocity.y = jumpVelocity;
				playerOne.jumpKeyUp = false;
			}
		}
	}
	
	if (state[SDL_SCANCODE_DOWN])
	{
		playerTwo.velocity.y = 2;
	}
	if (state[SDL_SCANCODE_UP])
	{
	
	}
	if (state[SDL_SCANCODE_RIGHT])
	{
		playerTwo.velocity.x = xSpeed;
	}
	if (state[SDL_SCANCODE_LEFT])
	{
		playerTwo.velocity.x = -xSpeed;
	}
	if (state[SDL_SCANCODE_L] && playerTwo.jumpReset > 0)
	{
		if (playerTwo.jumpReset == 3 && playerTwo.jumpKeyUp)
		{
			
			playerTwo.startTime = (SDL_GetTicks() / 50);
			playerTwo.jumpReset = playerTwo.jumpReset - 1;
			playerTwo.velocity.y = jumpVelocity;
			playerTwo.jumpKeyUp = false;
		}

		if (playerTwo.jumpReset == 2 && playerTwo.jumpKeyUp)
		{
			if ((playerTwo.startTime + 5 )< (SDL_GetTicks() / 50)) {
				playerTwo.startTime = (SDL_GetTicks() / 50);
				playerTwo.jumpReset = playerTwo.jumpReset - 1;
				playerTwo.velocity.y = jumpVelocity;
				playerTwo.jumpKeyUp = false;
			}
		}
		if (playerTwo.jumpReset == 1 && playerTwo.jumpKeyUp)
		{
			if ((playerTwo.startTime + 5 )< (SDL_GetTicks() / 50)) {
				playerTwo.jumpReset = playerTwo.jumpReset - 1;
				playerTwo.velocity.y = jumpVelocity;
				playerTwo.jumpKeyUp = false;
			}
		}
	}
}

Character Game::UpdatePlayer(Character x, SDL_Rect hitBox)
{
	{
		
		hitBox.y = x.location.y + x.velocity.y;
		hitBox.x = x.location.x;
		
		
		if (!checkCollision(hitBox))
		{
			x.location.y += x.velocity.y;
			x.velocity.y += gravity;
		}
		else
		{
			hitBox.y = x.location.y;
			x.velocity.y = 0;
			x.jumpReset = 3;
		}

		hitBox.x = x.location.x + x.velocity.x;
		if (!checkCollision(hitBox))
		{
			x.location.x += x.velocity.x;
		}
		else
		{
			hitBox.x = x.location.x;
		}
		return x;
		/*if (x.isHit)
		{
			if (x.velocity.x > 0)
			{
				x.location.x += x.velocity.x;
				x.velocity.x -= 1;
			}
			else if (x.velocity.x < 0)
			{
				x.location.x += x.velocity.x;
				x.velocity.x += 1;
			}
		}*/
	}
}
//think about making a second update player function that deals with hit movement, dont be afraid to use as many vars as you need, can clean up later.
void Game::displayFrame(Character f)
{
	if (f.velocity.y < 0)
	{
		getFrameUpAndDown(1, f.location.x, f.location.y);
	}
	else if (f.velocity.x > 0)
	{
		f.frameToDraw = ((SDL_GetTicks() - f.startTime) * 10 / 1000) % 4;

		getFrameRight(f.frameToDraw, f.location.x, f.location.y);
	}
	else if (f.velocity.x < 0)
	{
		f.frameToDraw = ((SDL_GetTicks() - f.startTime) * 10 / 1000) % 4;
		getFrameLeft(f.frameToDraw, f.location.x, f.location.y);
	}
	else
		getFrameUpAndDown(0, f.location.x, f.location.y);
}

void Game::GenerateOutput()
{
	if (playerTwo.health > 100) {
		free();
		close();
		mIsRunning = false;
	}
	SDL_RenderClear(gRenderer);

	SDL_RenderCopy(gRenderer, tBackground, NULL, NULL);

	SDL_RenderCopy(gRenderer, mTexture, NULL, NULL);

	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0);

	displayFrame(playerOne);

	displayFrame(playerTwo);

	SDL_RenderFillRect(gRenderer, &wall[0]);
	
	SDL_RenderFillRect(gRenderer, &wall[1]);
	
	SDL_RenderFillRect(gRenderer, &wall[3]);

	SDL_RenderFillRect(gRenderer, &playerOneAttackHitbox[0]);

	SDL_RenderFillRect(gRenderer, &playerOneAttackHitbox[1]);

	SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0);

	SDL_RenderPresent(gRenderer);
}

