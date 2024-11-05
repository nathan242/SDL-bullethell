#include "engine.h"
#include "ship.h"
#include "enemy.h"
#include "enemy_diagonal.h"
#include "enemy_diagonal_stationary.h"
#include "enemy_diagonal_stationary_fwdsprd.h"
#include "enemy_cargo.h"
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

#define ENEMY_SET_COUNT 5
#define ENEMY_SET_SIZE 5

bool game_over = false;

void activate_enemy_set(enemy *enemy_set[ENEMY_SET_SIZE], int set_id)
{
    switch (set_id)
    {
        case 0:
            enemy_set[0]->pos_x = 150;
            enemy_set[0]->pos_y = enemy_set[0]->size_y*-1;
            enemy_set[0]->step_x = 1;
            enemy_set[0]->step_y = 1;
            enemy_set[0]->draw_active = true;
            enemy_set[0]->phys_active = true;
            enemy_set[0]->move_x_last = {0, 0};
            enemy_set[0]->move_y_last = {0, 0};
            enemy_set[1]->pos_x = 300;
            enemy_set[1]->pos_y = enemy_set[1]->size_y*-2;
            enemy_set[1]->step_x = -1;
            enemy_set[1]->step_y = 1;
            enemy_set[1]->draw_active = true;
            enemy_set[1]->phys_active = true;
            enemy_set[1]->move_x_last = {0, 0};
            enemy_set[1]->move_y_last = {0, 0};
            enemy_set[2]->pos_x = 650;
            enemy_set[2]->pos_y = enemy_set[2]->size_y*-3;
            enemy_set[2]->step_x = 0;
            enemy_set[2]->step_y = 1;
            enemy_set[2]->draw_active = true;
            enemy_set[2]->phys_active = true;
            enemy_set[2]->move_x_last = {0, 0};
            enemy_set[2]->move_y_last = {0, 0};

            break;

        case 1:
            enemy_set[0]->pos_x = 150;
            enemy_set[0]->pos_y = enemy_set[0]->size_y*-1;
            enemy_set[0]->step_x = 1;
            enemy_set[0]->step_y = 1;
            enemy_set[0]->draw_active = true;
            enemy_set[0]->phys_active = true;
            enemy_set[0]->move_x_last = {0, 0};
            enemy_set[0]->move_y_last = {0, 0};
            enemy_set[1]->pos_x = 300;
            enemy_set[1]->pos_y = enemy_set[1]->size_y*-3;
            enemy_set[1]->step_x = -1;
            enemy_set[1]->step_y = 1;
            enemy_set[1]->draw_active = true;
            enemy_set[1]->phys_active = true;
            enemy_set[1]->move_x_last = {0, 0};
            enemy_set[1]->move_y_last = {0, 0};
            enemy_set[2]->pos_x = 100;
            enemy_set[2]->pos_y = enemy_set[2]->size_y*-5;
            enemy_set[2]->step_x = -1;
            enemy_set[2]->step_y = 1;
            enemy_set[2]->draw_active = true;
            enemy_set[2]->phys_active = true;
            enemy_set[2]->move_x_last = {0, 0};
            enemy_set[2]->move_y_last = {0, 0};

            break;

        case 2:
            enemy_set[0]->pos_x = 150;
            enemy_set[0]->pos_y = enemy_set[0]->size_y*-1;
            enemy_set[0]->step_x = 0;
            enemy_set[0]->step_y = 1;
            enemy_set[0]->draw_active = true;
            enemy_set[0]->phys_active = true;
            enemy_set[0]->move_x_last = {0, 0};
            enemy_set[0]->move_y_last = {0, 0};
            enemy_set[1]->pos_x = 550;
            enemy_set[1]->pos_y = enemy_set[0]->size_y*-4;
            enemy_set[1]->step_x = 0;
            enemy_set[1]->step_y = 1;
            enemy_set[1]->draw_active = true;
            enemy_set[1]->phys_active = true;
            enemy_set[1]->move_x_last = {0, 0};
            enemy_set[1]->move_y_last = {0, 0};

            break;

        case 3:
            enemy_set[0]->pos_x = 150;
            enemy_set[0]->pos_y = enemy_set[0]->size_y*-1;
            enemy_set[0]->step_x = 0;
            enemy_set[0]->step_y = 1;
            enemy_set[0]->draw_active = true;
            enemy_set[0]->phys_active = true;
            enemy_set[0]->move_x_last = {0, 0};
            enemy_set[0]->move_y_last = {0, 0};
            enemy_set[1]->pos_x = 550;
            enemy_set[1]->pos_y = enemy_set[0]->size_y*-4;
            enemy_set[1]->step_x = 0;
            enemy_set[1]->step_y = 1;
            enemy_set[1]->draw_active = true;
            enemy_set[1]->phys_active = true;
            enemy_set[1]->move_x_last = {0, 0};
            enemy_set[1]->move_y_last = {0, 0};

            break;
    }
}

void init_all_enemy_sets(enemy *enemy_sets[ENEMY_SET_COUNT][ENEMY_SET_SIZE], engine *eng, projectile_manager *enemy_shot_mngr)
{
    enemy_sets[0][0] = new enemy(eng, enemy_shot_mngr);
    enemy_sets[0][0]->init();
    eng->add_object(enemy_sets[0][0]);
    enemy_sets[0][1] = new enemy_diagonal(eng, enemy_shot_mngr);
    enemy_sets[0][1]->init();
    eng->add_object(enemy_sets[0][1]);
    enemy_sets[0][2] = new enemy_cargo(eng, enemy_shot_mngr);
    enemy_sets[0][2]->init();
    eng->add_object(enemy_sets[0][2]);

    enemy_sets[1][0] = new enemy_diagonal(eng, enemy_shot_mngr);
    enemy_sets[1][0]->init();
    eng->add_object(enemy_sets[1][0]);
    enemy_sets[1][1] = new enemy_diagonal(eng, enemy_shot_mngr);
    enemy_sets[1][1]->init();
    eng->add_object(enemy_sets[1][1]);
    enemy_sets[1][2] = new enemy_diagonal(eng, enemy_shot_mngr);
    enemy_sets[1][2]->init();
    eng->add_object(enemy_sets[1][2]);

    enemy_sets[2][0] = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
    enemy_sets[2][0]->init();
    eng->add_object(enemy_sets[2][0]);
    enemy_sets[2][1] = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
    enemy_sets[2][1]->init();
    eng->add_object(enemy_sets[2][1]);

    enemy_sets[3][0] = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
    enemy_sets[3][0]->init();
    eng->add_object(enemy_sets[3][0]);
    enemy_sets[3][1] = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
    enemy_sets[3][1]->init();
    eng->add_object(enemy_sets[3][1]);
}

void shooter()
{
    bool quit = false;
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool fire = false;

    bool fired = false;

    int active_enemy_set = 0;
    bool init_enemy_set = false;

    SDL_Event input;

    char *base_path = SDL_GetBasePath();

    chdir(base_path);
    free(base_path);

    engine *eng = new engine("SDL SHOOTER", RES_X, RES_Y, BPP);

    // Game objects
    projectile_manager *player_shot_mngr = new projectile_manager();
    projectile_manager *enemy_shot_mngr = new projectile_manager();
    ship *ship_obj = new ship(eng, player_shot_mngr);
    player_projectile *shots[NUM_SHOTS];
    enemy_projectile *enemy_shots[NUM_SHOTS_ENEMY];

    enemy *enemy_sets[ENEMY_SET_COUNT][ENEMY_SET_SIZE] = {};

    eng->add_object(ship_obj);
    ship_obj->init();

    init_all_enemy_sets(enemy_sets, eng, enemy_shot_mngr);
    activate_enemy_set(enemy_sets[active_enemy_set], active_enemy_set);

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        shots[shot_count] = new player_projectile();
        shots[shot_count]->init();
        eng->add_object(shots[shot_count]);
        player_shot_mngr->add_object(shots[shot_count]);
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        // Shots
        enemy_shots[enemy_shot_count] = new enemy_projectile();
        enemy_shots[enemy_shot_count]->init();
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

            init_enemy_set = true;
            for (int i = 0; i < ENEMY_SET_SIZE; i++) {
                if (enemy_sets[active_enemy_set][i] == NULL) continue;

                if (enemy_sets[active_enemy_set][i]->draw_active) {
                    init_enemy_set = false;
                    break;
                }
            }

            if (init_enemy_set && ++active_enemy_set == ENEMY_SET_COUNT) {
                active_enemy_set = 0;
            }

            if (init_enemy_set) {
                activate_enemy_set(enemy_sets[active_enemy_set], active_enemy_set);
            }

            // Redraw screen
            eng->step();
        }
    }

    SDL_Quit();

    delete eng;
    delete ship_obj;
    delete player_shot_mngr;
    delete enemy_shot_mngr;

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        delete shots[shot_count];
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        delete enemy_shots[enemy_shot_count];
    }

    for (int s = 0; s < ENEMY_SET_COUNT; s++) {
        for (int i = 0; i < ENEMY_SET_SIZE; i++) {
            if (enemy_sets[s][i] == NULL) continue;

            delete enemy_sets[s][i];
        }
    }

    return;
}

int main (int argc, char *argv[])
{
    shooter();
    return 0;
}

