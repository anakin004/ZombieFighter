#pragma once


#ifdef DEBUG
#define DEBUG_PRINT(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define DEBUG_PRINT(fmt, ...) ((void)0)
#endif



// Include necessary libraries
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

// Include your own headers
#include "LTexture.h"
#include "ball.h"

extern const bool RUNNING;

// External declarations of global variables
extern const int WIDTH;
extern const int HEIGHT;
extern const int FPS ;
extern const int FRAME_DELAY;

// sound effect for shooting
extern Mix_Chunk* shoot;
extern Mix_Chunk* wave;
extern Mix_Chunk* gameover;

//window and renderer
extern SDL_Window* gWindow;
extern SDL_Renderer* gRenderer;

//Globally used font
extern TTF_Font* gFont;

//Rendered texture
extern LTexture gTextTexture;
extern LTexture gWaveTexture;

// enemy
extern LTexture enemy;

// player
extern LTexture player;

//end
extern LTexture end;

// bullet
extern Ball ball;