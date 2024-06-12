#pragma once

#include <SDL.h>
#include <string>
#include <SDL_ttf.h>
#include <vector>

class LTexture {
public:

    LTexture();
    ~LTexture();
    bool loadFromFile(std::string path);
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
    void free();
    void render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center );
    int getWidth();
    int getHeight();
    double get_rot( int sprite_x , int sprite_y , int player_x, int player_y);
    std::vector<double> get_distances( int sx, int sy, int px, int py);
    
    float vx;
    float vy;

private:
    SDL_Texture* mTexture;
    int mWidth;
    int mHeight;
};



