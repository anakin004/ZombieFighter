#include "ball.h"
#include "gamestate.h"
#include <cmath>

Ball::Ball() {
    vx = 5;
    vy = 5;

}



double Ball::get_degrease( int sprite_x, int sprite_y, int mouse_x, int mouse_y ){
    
    double angle = 0;


    int x=0, y=0;
    y = abs(mouse_y-sprite_y);
    x = abs(mouse_x-sprite_x);

    
    
    if ( mouse_x >= sprite_x && mouse_y <= sprite_y ){
        if ( x == 0 )
            angle = 0;
        else 
            angle = 90 - atan((double)y/x)*(180/M_PI);
    
    }
    else if ( mouse_x < sprite_x && mouse_y <= sprite_y ){

        if (x == 0 )
            angle = 0;

        else 
            angle = (90 - atan((double)y/x)*(180/M_PI))*-1;
    }

    else if ( mouse_x < sprite_x && mouse_y >= sprite_y ){
        if ( x == 0 )
            angle = 180;
        
        else
            angle =  atan((double)y/x)*(180/M_PI)*-1 -90;
    }


    else {
        if ( x == 0 )
            angle = 180;
        else
            angle = atan((double)y/x)*(180/M_PI) + 90;
    }
    return angle;
}

std::vector<double> Ball::gun_pos( double angle, int hyp ){

    double x, y;
    double temp_a = angle;

    if( angle >= 0 && angle <= 90 )
        angle = 90-angle;

    else if ( angle > 90 && angle < 180 )
        angle = 90+angle;

    else if ( angle < 0 && angle >= -90 )
        angle = 90-angle;
    
    else if ( angle < -90 && angle >= -180)
        angle = 90+angle;


    y=sin(angle*M_PI/180)*hyp * -1 ;
    x=cos(angle*M_PI/180)*hyp;

    if ( temp_a < -90 || temp_a > 90 )
        x*=-1;

    return {x,y};



}


std::vector<double> Ball::normalize(std::vector<double> vec) {
    double length = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
    if (length != 0) {
        vec[0] /= length;
        vec[1] /= length;
    }
    return vec;
}