
#include "gamestate.h"


Game APP;

int main( int argc, char* args[] )
{
    while( APP.getState() == RUNNING ){
        APP.run();
    }
   
    close();
    return EXIT_SUCCESS;
}




