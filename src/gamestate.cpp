#include "gamestate.h"
#include "globals.h"
#include "init.h"
#include <cmath>
#include <memory>



Point generateRandomPoint(std::mt19937& eng) {
    std::uniform_int_distribution<> rangeSelector(0, 3); // To select one of the four ranges
    int range = rangeSelector(eng);

    if (range == 0) {
        // Range 1: x = 0, y = 0 to HEIGHT
        std::uniform_int_distribution<> distrY(0, HEIGHT);
        return {0, distrY(eng)};
    } else if (range == 1) {
        // Range 2: x = WIDTH, y = 0 to HEIGHT
        std::uniform_int_distribution<> distrY(0, HEIGHT);
        return {WIDTH, distrY(eng)};
    } else if (range == 2) {
        // Range 3: y = 0, x = 0 to WIDTH
        std::uniform_int_distribution<> distrX(0, WIDTH);
        return {distrX(eng), 0};
    } else {
        // Range 4: y = HEIGHT, x = 0 to WIDTH
        std::uniform_int_distribution<> distrX(0, WIDTH);
        return {distrX(eng), HEIGHT};
    }
}


Game::Game(){
    running = true;
}



void Game::run() {


    if (!init()) {
        printf("Failed to initialize!\n");
    } 
    
    else {

        if (!loadMedia()) {
            printf("Failed to load media!\n");
        }

        else{

            SDL_Event e;

        

            bool quit = false;
            bool shot = false;
            bool add_shot = false;
            bool prev_click = false;
            bool render_enemy = false;
            bool dead = false;
            bool start_round = true;
            int points = 0;
            int alive = 0;
            int zombCount = 5;
            int wave_count = 0;

            static std::random_device rd;  // Obtain a random number from hardware
            static std::mt19937 eng(rd()); // Seed the generator


            double angle = 0;
            int x = 0, y = 0;

            // will hold the return of the get offset func
            static std::vector<double> gun;

            //will hold bullet positions
            static std::list<std::vector<double>> bullets;

            // will hold angles and enemy x and y
            static std::vector<double> enemy_vec;
            
            //ptr so we can deallocate the zombiez when game ends
            static std::unique_ptr<std::list<std::vector<double>>> enemy_pos_ptr(new std::list<std::vector<double>>);
            if ( enemy_pos_ptr == NULL ){
                printf("faield to allocate memory for enemy pos ");
                quit = true;
            }


            //will hold normalls for movement
            static std::vector<double> normal;

            // making points for where they spawn
            static std::unique_ptr<std::vector<Point>> pointz(new std::vector<Point>);
            if ( pointz == NULL ){
                printf("faield to allocate memory for pointz");
                quit = true;
            }


            Mix_PlayChannel( -1, wave, 0 );

            while( !quit )
            {   

                Uint32 frameStart = SDL_GetTicks();
                    
                while( SDL_PollEvent(&e) != 0 ){

                    if( e.type == SDL_QUIT ){
                        quit = true;
                        running = false;
                        break;
                    }

                    

                    if ( e.type == SDL_MOUSEBUTTONDOWN ){
    
                        
                        if ( e.button.button == SDL_BUTTON_LEFT  && prev_click == false ){
                            shot = true;
                            add_shot = true;
                            prev_click = true;
                            }
                        
                        else if ( e.button.button == SDL_BUTTON_RIGHT ){
                            render_enemy = true;
                        }
                    }

                    else if ( e.type == SDL_MOUSEBUTTONUP ){
                        prev_click = false;
                    
                    }

            
                    else{
                        add_shot = false;
                    }
                    
                }








                // if we die we wait til user clicks then restarts
                if (dead) {
                    die();
                    //play gameover sound
                    Mix_PlayChannel( -1, gameover, 0 );
                    SDL_Delay( 11000 );
                    SDL_Event event;
                    while (SDL_WaitEvent(&event)) {
                        if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_QUIT) {
                            // Reset dead flag and any other relevant variables
                            dead = false;
                            // Reset the enemy position list
                            enemy_pos_ptr.reset(new std::list<std::vector<double>>);
                            pointz.reset(new std::vector<Point>);

                            wave_count = 0;
                            shot = false;
                            add_shot = false;
                            prev_click = false;
                            render_enemy = true;
                            alive = 0;
                            zombCount = 5;
                            start_round = true;
                            
                            break; // Exit the event loop
                        }
                    }
                    continue; // Skip the rest of the loop and start over
                }







                SDL_SetRenderDrawColor( gRenderer, 117, 140, 122, 0 );
                SDL_RenderClear( gRenderer );


                // if we define -DDEBUG in makefile then this will print else it will remove it
                DEBUG_PRINT("HERE");



        
                SDL_Rect player_clip = {0,0, 50, 80};
                SDL_Point player_p; player_p.y = 52; player_p.x = 25;
                player.render( WIDTH/2 -25, HEIGHT/2 -40, &player_clip, angle, &player_p );

                std::string pts = "Points " + std::to_string(points);
                std::string wave_BRUH = "Wave " + std::to_string(wave_count);
                //std::string pts2 = std::to_string(ball_sprite.pts_2);
                
                SDL_Rect text_clip = {0,0,70,20};
                SDL_Rect text2_clip = {0,0,70,20};
                gTextTexture.loadFromRenderedText(pts, {255,255,255});
                gWaveTexture.loadFromRenderedText(wave_BRUH, {255,255,255});


                gTextTexture.render(20, 10, &text_clip, 0, NULL);
                gWaveTexture.render(WIDTH-80, 10, &text2_clip, 0, NULL);

                SDL_GetMouseState( &x, &y );

                SDL_Rect ball_clip = {0,0, 10, 10};
                angle = ball.get_degrease( WIDTH/2 +25, HEIGHT/2 + 10, x, y );                    
                gun = ball.gun_pos( angle, 40);
                


                //shooting

                if( shot == true && frameStart > 12000  ){

                    if ( add_shot == true && prev_click == true){
                        Mix_PlayChannel( -1, shoot, 0 );
                        bullets.push_back(gun);
                    }
            
                    
                    // if a bullet gets erased the list can become empty;
                    if ( !bullets.empty() ){

                    for( auto it = bullets.begin(); it != bullets.end() ; it++ ){
                    

                        if( abs( (*it)[0] ) > 900/2 ||  abs( (*it)[1] ) > 650/2 ) 
                            bullets.erase(it);

                        normal = ball.normalize( *it );
                        ball.render( WIDTH/2 - 5 + (*it)[0], HEIGHT/2 + 5 + (*it)[1], &ball_clip, 0, NULL );
                        //update ball pos for next frame
                        (*it)[0] += normal[0] * ball.vx;
                        (*it)[1] += normal[1] * ball.vy;

                    }

                    }

                    prev_click = false;
                }








                        //ZAMBIES

                
                if ( alive == 0 && !dead && frameStart > 12000 ){
                    pointz.reset(new std::vector<Point>);
                    zombCount += 1;
                    wave_count += 1;
                    for (int i = 0; i < zombCount; ++i) { // Example: Generate 10 points
                        start_round = true;
                        pointz->push_back(generateRandomPoint(eng));
                    }
                }
 
                SDL_Rect enemy_clip = {0,0, 70, 70 };
                SDL_Point enemy_center; enemy_center.x = 70/2; enemy_center.y = 70/2;

                if ( !dead && frameStart > 12000  ){

                    if ( start_round == true ){
                        
                        for( auto pointzIt = pointz->begin(); pointzIt != pointz->end() ; pointzIt++){
                            double enemy_angle = enemy.get_rot( pointzIt->x + 70/2, pointzIt->y + 70/2, WIDTH/2-25, HEIGHT/2 - 10 );
                            enemy_pos_ptr->push_back( {(double)pointzIt->x + 70/2,(double)pointzIt->y + 70/2, enemy_angle} );
                        }
                        //printf("ZOMb counter inside %d\n", zombCount);
                        zombCount = enemy_pos_ptr->size();
                        alive = zombCount;
                        start_round = false;
                    }
                    
                        // goes through all the zombie positions and checks if its  touching player pos
                        // if it is then we are dead
                    if ( !enemy_pos_ptr -> empty() ){
                    for( auto it = enemy_pos_ptr->begin(); it != enemy_pos_ptr->end() ; it++ ){

                        //rendering zombie while in for loop, the enemyy pos ptr points to a
                        // list of vectors that is storiing renderng positions
                        enemy.render( (*it)[0], (*it)[1] ,  &enemy_clip, (*it)[2] , &enemy_center);
                        enemy_vec = enemy.get_distances( (int)(*it)[0], (int)(*it)[1], WIDTH/2 - 25, HEIGHT/2 - 10 );
                        normal = ball.normalize(enemy_vec);
                        //enemy vec is a double vec that is a different in x and y to center
                        if ( enemy_vec[0] == 0 || enemy_vec[1] == 0 ){
                            dead = true;
                            points = 0;
                            enemy_pos_ptr.reset();
                            break;
                        }
                        // if we arent touching we advance towards center

                        (*it)[0] += normal[0]/2;
                        (*it)[1] += normal[1]/2;
                    }
                    }

                    else{
                        start_round = true;
                    }
                  
                }



                // Loop through bullets

                if ( shot && !dead && frameStart > 12000  ){

                    
                for (auto bullet_it = bullets.begin(); bullet_it != bullets.end();) {

                    bool bullet_hit = false; // Flag to indicate if the bullet hits a zombie

                    // Loop through zombies
                    if ( enemy_pos_ptr->empty()){
                        break;
                    }
                    for (auto zombie_it = enemy_pos_ptr->begin(); zombie_it != enemy_pos_ptr->end(); ++zombie_it) {
                        // Calculate distance between bullet and zombie

                        double distance = sqrt(pow(((((*bullet_it)[0] + 450 ) * WIDTH)/ 900) - (*zombie_it)[0]+10, 2) + pow(((((*bullet_it)[1] + 350 ) * HEIGHT)/ 650) - (*zombie_it)[1]-50, 2));
                        // If distance is less than a threshold (e.g., bullet radius + zombie radius), the bullet hits the zombie
                        if (distance < 50 ) {
                            bullet_hit = true;
                            // Remove bullet from the list
                            bullet_it = bullets.erase(bullet_it);
                            zombie_it = enemy_pos_ptr->erase(zombie_it);

                            alive -=1;

                            points += 1;

                            break; // Exit the zombie loop since the bullet can only hit one zombie
                        }
                    }

                    // If the bullet did not hit any zombie, move to the next bullet
                    if (!bullet_hit) {
                        ++bullet_it;
                    }
                }

            }


                SDL_RenderPresent( gRenderer );

                Uint32 frameTime = SDL_GetTicks() - frameStart;

        

                if (frameTime < FRAME_DELAY) 
                    SDL_Delay(FRAME_DELAY - frameTime);
                


            }
        }
    }
}

bool Game::getState(){
    return running;
}

void Game::die(){
    SDL_SetRenderDrawColor( gRenderer, 117, 140, 122, 0 );
    SDL_RenderClear( gRenderer );
    end.render( 0, 0, NULL, 0, NULL);
    SDL_RenderPresent( gRenderer );
}


void close() {
    player.free();
    ball.free();
    enemy.free();
    end.free();
    gTextTexture.free();

    Mix_FreeChunk( shoot );
    Mix_FreeChunk( wave );
    shoot = NULL;
    wave = NULL;

    TTF_CloseFont( gFont );
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gFont = NULL;
    gWindow = NULL;
    gRenderer = NULL;

    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();
}