#pragma once

// Include necessary libraries
#include <SDL.h>
#include <vector>
#include <string>
#include <list>
#include <random>

// Forward declarations
class LTexture;
class Ball;

// Include your own headers

#include "globals.h"

class Game {
public:

    Game();

    void run();

    bool getState();

    void die();

private:
    
    bool running;
    
};

// for random seed generation
struct Point {
    int x, y;
};


Point generateRandomPoint(std::mt19937& eng);

void close();