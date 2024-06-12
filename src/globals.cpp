#include "globals.h"

const bool RUNNING = true;

const int WIDTH = 900;
const int HEIGHT = 650;
const int FPS = 144;
const int FRAME_DELAY = 1000 / FPS;

// sound effect for shooting
Mix_Chunk* shoot = NULL;
Mix_Chunk* wave = NULL;
Mix_Chunk* gameover = NULL;

//window and renderer
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;

//Rendered texture
LTexture gTextTexture;
LTexture gWaveTexture;

// enemy
LTexture enemy;

//game over
LTexture end;

// player
LTexture player;

// bullet
Ball ball;