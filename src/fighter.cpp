
#include "gamestate.h"


Game ZEST;

int main( int argc, char* args[] )
{
    while( ZEST.getState() == RUNNING ){
        ZEST.run();
    }
   
    close();
    return EXIT_SUCCESS;
}




