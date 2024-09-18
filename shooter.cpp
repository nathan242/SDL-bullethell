#include "engine.h"
#include "ship.h"
#include "enemy.h"
#include "player_projectile.h"
#include "enemy_projectile.h"
#include "projectile_manager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <unistd.h>

#define RES_X 800
#define RES_Y 600
#define BPP 32

int ID_PLAYER_SHIP = 1;
int ID_ENEMY_SHIP = 2;
int ID_PLAYER_SHOT = 1000;
int ID_ENEMY_SHOT = 1001;
int SHOT_PHYS_DELAY = 2500000;

#define NUM_SHOTS 40
#define NUM_SHOTS_ENEMY 40

#define ENEMY_SHOT_DELAY 500000000

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
    projectile_manager *player_shot_mngr = new projectile_manager();
    projectile_manager *enemy_shot_mngr = new projectile_manager();
    ship *ship_obj = new ship(eng, player_shot_mngr);
    enemy *enemy_obj = new enemy(eng, enemy_shot_mngr);
    player_projectile *shots[NUM_SHOTS];
    enemy_projectile *enemy_shots[NUM_SHOTS_ENEMY];

    eng->add_object(ship_obj);
    eng->add_object(enemy_obj);

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        shots[shot_count] = new player_projectile();

        eng->add_object(shots[shot_count]);
        player_shot_mngr->add_object(shots[shot_count]);
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        // Shots
        enemy_shots[enemy_shot_count] = new enemy_projectile();

        eng->add_object(enemy_shots[enemy_shot_count]);
        enemy_shot_mngr->add_object(enemy_shots[enemy_shot_count]);
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
            ship_obj->step_x = 0;
            ship_obj->step_y = 0;
            if (left) { ship_obj->step_x = -1; }
            if (right) { ship_obj->step_x = 1; }
            if (up) { ship_obj->step_y = -1; }
            if (down) { ship_obj->step_y = 1; }

            if (fire) {
                if (!fired) {
                    fired = true;
                    ship_obj->fire();
                }
            } else {
                fired = false;
            }

            if (reset_enemy) {
                reset_enemy = false;
                enemy_obj->pos_x = 150;
                enemy_obj->pos_y = 150;
                enemy_obj->step_x = 1;
            }

            clock_gettime(CLOCK_MONOTONIC, &now);
            timediff = ((now.tv_sec - last_enemy_shot.tv_sec) * 1000000000) + (now.tv_nsec - last_enemy_shot.tv_nsec);

            if (timediff > ENEMY_SHOT_DELAY) {
                enemy_obj->fire();
                last_enemy_shot = now;
            }

            // Redraw screen
            eng->step();
        }
    }

    SDL_Quit();

    delete eng;

    delete ship_obj;
    delete enemy_obj;

    delete player_shot_mngr;
    delete enemy_shot_mngr;

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        delete shots[shot_count];
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        delete enemy_shots[enemy_shot_count];
    }

    return;
}

int main (int argc, char *argv[])
{
    shooter();
    return 0;
}

