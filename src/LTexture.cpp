#include "globals.h"
#include <cmath>

LTexture::LTexture() {
    vx = 0;
    vy = 0;
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture() {
    free();
}

void LTexture::free()
{
    //Free texture if it exists
    if( mTexture != NULL )
    {
        SDL_DestroyTexture( mTexture );
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

int LTexture::getWidth() {
    return mWidth;
}

int LTexture::getHeight() {
    return mHeight;
}


bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
    //Get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
    if( textSurface == NULL )
    {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    }
    else
    {
        //Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
        if( mTexture == NULL )
        {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        }
        else
        {
            mWidth = 30;
            mHeight = 30;
        }

        //Get rid of old surface
        SDL_FreeSurface( textSurface );
    }
    
    //Return success
    return mTexture != NULL;
}


void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, WIDTH, HEIGHT };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    if ( center == NULL )
        SDL_RenderCopy( gRenderer, mTexture, NULL , &renderQuad);
    
    else
        SDL_RenderCopyEx( gRenderer, mTexture, NULL, &renderQuad, angle, center, SDL_FLIP_NONE );

}



bool LTexture::loadFromFile( std::string path )
{
    //Get rid of preexisting texture
    free();
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );

    SDL_Surface* src = SDL_GetWindowSurface( gWindow );

    SDL_Surface* optimized = NULL;

    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        optimized = SDL_ConvertSurface ( loadedSurface, src->format, 0 );
        
        if( optimized == NULL ){
            printf("Cannot optimize");
        }
        else{
            // ball.png backround is black so we make that transparent so it doesnt override the lines we draw, the ball goes to the width and height so r = width and r = height (radius)
            if ( path == "ball.png")
                SDL_SetColorKey( optimized, SDL_TRUE, SDL_MapRGB( optimized->format, 0, 0, 0 ) );
            else if ( path == "zombie.png" )
                SDL_SetColorKey( optimized, SDL_TRUE, SDL_MapRGB( optimized->format, 255, 255, 255 ) );

            //Create texture from surface pixels
            newTexture = SDL_CreateTextureFromSurface( gRenderer, optimized );
            if( newTexture == NULL )
            {
                printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            }
            else
            {

                //Get image dimensions
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            //Get rid of old loaded surface
            SDL_FreeSurface( loadedSurface );
            SDL_FreeSurface( optimized );

        }
    }

    //Return success
    mTexture = newTexture;
    return mTexture != NULL;
}



double LTexture::get_rot( int sprite_x , int sprite_y , int player_x, int player_y){
    
    double angle = 0;


    int x=0, y=0;
    y = abs(sprite_y-player_y);
    x = abs(sprite_x-player_x);

    
    
    if ( sprite_x >= player_x && sprite_y <= player_y ){
        if ( x == 0 )
            angle = 0;
        else 
            angle = (180 - atan((double)x/y)*(180/M_PI))*-1;
    
    }
    else if ( sprite_x < player_x && sprite_y <= player_y ){

        if (x == 0 )
            angle = 0;

        else 
            angle = (180 - atan((double)x/y)*(180/M_PI));
    }

    else if ( sprite_x < player_x && sprite_y >= player_y ){
        if ( x == 0 )
            angle = 180;
        
        else
            angle =  atan((double)x/y)*(180/M_PI);
    }


    else {
        if ( x == 0 )
            angle = 180;
        else
            angle = atan((double)x/y)*(180/M_PI)*-1;
    }
    return angle;
}

std::vector<double> LTexture::get_distances( int sx, int sy, int px, int py){

    
    double dx = abs( (double) sx - px ) ;
    double dy = abs( (double) sy - py ) ;


    if ( sx > px )
        dx*=-1;
    if ( sy > py )
        dy*=-1;


    if( (abs(dx) < 20  && abs(dx) > 0) && (abs(dy) < 20 && abs(dy) > 0)){
        dx = 0;
        dy = 0;
    }


    return  {dx/144, dy/144}; 

}