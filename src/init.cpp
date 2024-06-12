#include "init.h"
#include "globals.h"
#include <stdio.h>

bool init() {
    bool success = true;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0) {
        success = false;
        printf("Couldn't initialize video");
    } else {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1")) {
            printf("Warning: Linear texture filtering not enabled!");
        }
        gWindow = SDL_CreateWindow("Zesty", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
                                   SDL_WINDOW_SHOWN);
        if (gWindow == NULL) {
            success = false;
            printf("Could not create window %s", SDL_GetError());
        } else {
            if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG  ) {
                success = false;
                printf(" wrong goofy ");
            } else {
                gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
                if (gRenderer == NULL) {
                    printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                    success = false;
                } else{
                    SDL_SetRenderDrawColor(gRenderer, 117, 140, 122, 255);
                    
                    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
                    {
                        printf( "SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError() );
                        success = false;
                    }
                    if( TTF_Init() == -1 )
                    {
                            printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                            success = false;
                    }

                }
            }
        }
    }
    return success;
}

bool loadMedia() {
    bool success = true;
    if (!player.loadFromFile("player.png")) {
        printf("Failed to load player texture image!\n");
        success = false;
    }
    if (!ball.loadFromFile("ball.png")) {
        printf("Failed to load ball texture image!\n");
        success = false;
    }

    if (!enemy.loadFromFile("zombie.png")) {
        printf("Failed to load zombie texture image!\n");
        success = false;
    }
    if (!end.loadFromFile("gameover.png")) {
        printf("Failed to load over texture image!\n");
        success = false;
    }

    gFont = TTF_OpenFont( "lazy.ttf", 28 );
    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }

    shoot = Mix_LoadWAV( "blaster.wav" );
    if ( shoot == NULL ){
        success = false;
    }

    wave = Mix_LoadWAV( "wave.wav" );
    if( wave == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }

    gameover = Mix_LoadWAV( "end.wav" );
    if( gameover == NULL )
    {
        printf( "Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError() );
        success = false;
    }


    return success;

}