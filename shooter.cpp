#include "engine.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <unistd.h>

#define RES_X 800
#define RES_Y 600
#define BPP 32

#define ID_PLAYER_SHIP 1
#define ID_ENEMY_SHIP 2
#define ID_PLAYER_SHOT 1000
#define ID_ENEMY_SHOT 1001

#define NUM_SHOTS 40
#define NUM_SHOTS_ENEMY 40

#define ENEMY_SHOT_DELAY 500000000

#define SHIP_MOVE_PHYS_DELAY 3000000
#define ENEMY_SHIP_MOVE_PHYS_DELAY 4000000
#define SHOT_PHYS_DELAY 4000000

bool reset_enemy = false;
bool game_over = false;

timespec inittime {0, 0};

void shot_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    obj->phys_active = false;
    obj->draw_active = false;

    if (obj2 != NULL) {
        if (obj2->type_id >= ID_PLAYER_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        }
    }
}

void enemy_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT) {
            reset_enemy = true;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        }
    } else if (collide_axis == 2 && obj->pos_y > 0) {
        game_over = true;
    }
}

void shooter()
{
    // bool vars for control directions and quit event
    bool quit = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fire = false;

    bool fired = false;

    SDL_Event input;

    timespec last_enemy_shot {0, 0};
    timespec now;
    uint64_t timediff;

    char *base_path = SDL_GetBasePath();

    chdir(base_path);
    free(base_path);

    engine *eng = new engine("SDL SHOOTER", RES_X, RES_Y, BPP);

    // Game objects
    engine_obj *ship = new engine_obj;
    engine_obj *enemy = new engine_obj;
    engine_obj *shots[NUM_SHOTS];
    engine_obj *enemy_shots[NUM_SHOTS_ENEMY];

    // Ship
    ship->type_id = ID_PLAYER_SHIP;
    ship->pos_x = 50;
    ship->pos_y = 550;
    ship->size_x = 21;
    ship->size_y = 21;
    ship->phys_size_x = 21;
    ship->phys_size_y = 21;
    ship->step_x = 0;
    ship->step_y = 0;
    ship->move_x_every = SHIP_MOVE_PHYS_DELAY;
    ship->move_y_every = SHIP_MOVE_PHYS_DELAY;
    ship->move_x_last = inittime;
    ship->move_y_last = inittime;
    ship->bounce = 0;
    ship->collided = NULL;
    ship->callback = NULL;
    ship->phys_active = true;
    ship->draw_active = true;
    ship->sprite = IMG_Load("ship.png");
    ship->texture = SDL_CreateTextureFromSurface(eng->renderer, ship->sprite);
    ship->size_x = 21;
    ship->size_y = 21;

    eng->add_object(ship);

    // Enemy
    enemy->type_id = ID_ENEMY_SHIP;
    enemy->pos_x = 150;
    enemy->pos_y = 150;
    enemy->size_x = 20;
    enemy->size_y = 20;
    enemy->phys_size_x = 20;
    enemy->phys_size_y = 20;
    enemy->step_x = 1;
    enemy->step_y = 1;
    enemy->move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    enemy->move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    enemy->move_x_last = inittime;
    enemy->move_y_last = inittime;
    enemy->bounce = 1;
    enemy->collided = NULL;
    enemy->callback = enemy_callback;
    enemy->phys_active = true;
    enemy->draw_active = true;
    enemy->sprite = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
    enemy->size_x = 20;
    enemy->size_y = 20;
    SDL_FillRect(enemy->sprite, NULL, SDL_MapRGB(enemy->sprite->format, 255, 0, 0));
    enemy->texture = SDL_CreateTextureFromSurface(eng->renderer, enemy->sprite);

    eng->add_object(enemy);    

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        // Shots
        shots[shot_count] = new engine_obj;

        shots[shot_count]->type_id = ID_PLAYER_SHOT;
        shots[shot_count]->pos_x = ship->pos_x+(ship->size_x/2);
        shots[shot_count]->pos_y = ship->pos_y-ship->size_y;
        shots[shot_count]->size_x = 2;
        shots[shot_count]->size_y = 10;
        shots[shot_count]->phys_size_x = 2;
        shots[shot_count]->phys_size_y = 10;
        shots[shot_count]->step_x = 0;
        shots[shot_count]->step_y = -1;
        shots[shot_count]->move_x_every = SHOT_PHYS_DELAY;
        shots[shot_count]->move_y_every = SHOT_PHYS_DELAY;
        shots[shot_count]->move_x_last = inittime;
        shots[shot_count]->move_y_last = inittime;
        shots[shot_count]->bounce = -1;
        shots[shot_count]->collided = NULL;
        shots[shot_count]->callback = shot_callback;
        shots[shot_count]->phys_active = false;
        shots[shot_count]->draw_active = false;
        shots[shot_count]->sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
        shots[shot_count]->size_x = 2;
        shots[shot_count]->size_y = 10;
        SDL_FillRect(shots[shot_count]->sprite, NULL, SDL_MapRGB(shots[shot_count]->sprite->format, 0, 255, 0));
        shots[shot_count]->texture = SDL_CreateTextureFromSurface(eng->renderer, shots[shot_count]->sprite);

        eng->add_object(shots[shot_count]);
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        // Shots
        enemy_shots[enemy_shot_count] = new engine_obj;

        enemy_shots[enemy_shot_count]->type_id = ID_ENEMY_SHOT;
        enemy_shots[enemy_shot_count]->pos_x = enemy->pos_x+(enemy->size_x/2);
        enemy_shots[enemy_shot_count]->pos_y = enemy->pos_y+enemy->size_y+1;
        enemy_shots[enemy_shot_count]->size_x = 2;
        enemy_shots[enemy_shot_count]->size_y = 10;
        enemy_shots[enemy_shot_count]->phys_size_x = 2;
        enemy_shots[enemy_shot_count]->phys_size_y = 10;
        enemy_shots[enemy_shot_count]->step_x = 0;
        enemy_shots[enemy_shot_count]->step_y = 1;
        enemy_shots[enemy_shot_count]->move_x_every = SHOT_PHYS_DELAY;
        enemy_shots[enemy_shot_count]->move_y_every = SHOT_PHYS_DELAY;
        enemy_shots[enemy_shot_count]->move_x_last = inittime;
        enemy_shots[enemy_shot_count]->move_y_last = inittime;
        enemy_shots[enemy_shot_count]->bounce = -1;
        enemy_shots[enemy_shot_count]->collided = NULL;
        enemy_shots[enemy_shot_count]->callback = shot_callback;
        enemy_shots[enemy_shot_count]->phys_active = false;
        enemy_shots[enemy_shot_count]->draw_active = false;
        enemy_shots[enemy_shot_count]->sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
        enemy_shots[enemy_shot_count]->size_x = 2;
        enemy_shots[enemy_shot_count]->size_y = 10;
        SDL_FillRect(enemy_shots[enemy_shot_count]->sprite, NULL, SDL_MapRGB(enemy_shots[enemy_shot_count]->sprite->format, 180, 0, 0));
        enemy_shots[enemy_shot_count]->texture = SDL_CreateTextureFromSurface(eng->renderer, enemy_shots[enemy_shot_count]->sprite);

        eng->add_object(enemy_shots[enemy_shot_count]);
    }

    // Main loop
    while (quit==false)
    {
        // Read inputs
        while (SDL_PollEvent(&input))
        {
            switch (input.type)
            {
                case SDL_KEYDOWN:
                    switch (input.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                left = true;
                                break;
                            case SDLK_RIGHT:
                                right = true;
                                break;
                            case SDLK_UP:
                                up = true;
                                break;
                            case SDLK_DOWN:
                                down = true;
                                break;
                            case SDLK_SPACE:
                                fire = true;
                                break;
                            case SDLK_q:
                                quit = true;
                                break;
                        }
                        break;
                case SDL_KEYUP:
                    switch (input.key.keysym.sym)
                        {
                            case SDLK_LEFT:
                                left = false;
                                break;
                            case SDLK_RIGHT:
                                right = false;
                                break;
                            case SDLK_UP:
                                up = false;
                                break;
                            case SDLK_DOWN:
                                down = false;
                                break;
                            case SDLK_SPACE:
                                fire = false;
                                break;
                        }
            }
        }

        if (!game_over) {
            ship->step_x = 0;
            if (left) { ship->step_x = -1; }
            if (right) { ship->step_x = 1; }

            if (fire) {
                if (!fired) {
                    fired = true;
                    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
                        if (!shots[shot_count]->draw_active) {
                            shots[shot_count]->pos_x = ship->pos_x+(ship->size_x/2);
                            shots[shot_count]->pos_y = ship->pos_y-ship->size_y;
                            shots[shot_count]->phys_active = true;
                            shots[shot_count]->draw_active = true;
                            shots[shot_count]->move_x_last = inittime;
                            shots[shot_count]->move_y_last = inittime;

                            break;
                        }
                    }
                }
            } else {
                fired = false;
            }

            if (reset_enemy) {
                reset_enemy = false;
                enemy->pos_x = 150;
                enemy->pos_y = 150;
                enemy->step_x = 1;
            }

            clock_gettime(CLOCK_MONOTONIC, &now);
            timediff = ((now.tv_sec - last_enemy_shot.tv_sec) * 1000000000) + (now.tv_nsec - last_enemy_shot.tv_nsec);

            if (timediff > ENEMY_SHOT_DELAY) {
                for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
                    if (!enemy_shots[enemy_shot_count]->draw_active) {
                        enemy_shots[enemy_shot_count]->pos_x = enemy->pos_x+(enemy->size_x/2);
                        enemy_shots[enemy_shot_count]->pos_y = enemy->pos_y+enemy->size_y+1;
                        enemy_shots[enemy_shot_count]->phys_active = true;
                        enemy_shots[enemy_shot_count]->draw_active = true;
                        enemy_shots[enemy_shot_count]->move_x_last = inittime;
                        enemy_shots[enemy_shot_count]->move_y_last = inittime;

                        break;
                    }
                }

                last_enemy_shot = now;
            }

            // Redraw screen
            eng->step();
        }
    }

    SDL_Quit();

    delete eng;

    SDL_DestroyTexture(ship->texture);
    SDL_FreeSurface(ship->sprite);
    SDL_DestroyTexture(enemy->texture);
    SDL_FreeSurface(enemy->sprite);

    delete ship;
    delete enemy;

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        SDL_DestroyTexture(shots[shot_count]->texture);
        SDL_FreeSurface(shots[shot_count]->sprite);
        delete shots[shot_count];
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        SDL_DestroyTexture(enemy_shots[enemy_shot_count]->texture);
        SDL_FreeSurface(enemy_shots[enemy_shot_count]->sprite);
        delete enemy_shots[enemy_shot_count];
    }

    return;
}

int main (int argc, char *argv[])
{
    shooter();
    return 0;
}

