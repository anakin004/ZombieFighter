#pragma once

#include "LTexture.h"
#include <vector>
#include <list>




extern const int WIDTH;
extern const int HEIGHT;


class Ball: public LTexture {
public:
    Ball();

    void create_bullet( std::list<Ball> &bullets );

    double get_degrease( int sprite_x, int sprite_y, int mouse_x, int mouse_y );

    std::vector<double> gun_pos( double angle, int hyp );

    std::vector<double> normalize(std::vector<double> vec);
        
    float vx;    
    float vy;

};


