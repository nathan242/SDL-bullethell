#include "physics.h"
#include "graphics.h"
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

void shot_callback(phys_obj *obj, phys_obj *obj2, int collide_axis, int area_x, int area_y)
{
    obj->active = false;

    if (obj2 != NULL) {
        if (obj2->type_id >= ID_PLAYER_SHOT) {
            obj2->active = false;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        }
    }
}

void enemy_callback(phys_obj *obj, phys_obj *obj2, int collide_axis, int area_x, int area_y)
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

    graphics *window = new graphics("SDL SHOOTER", RES_X, RES_Y, BPP);

    // Game objects
    graphics_obj *ship = new graphics_obj;
    phys_obj *ship_phys = new phys_obj;
    graphics_obj *enemy = new graphics_obj;
    phys_obj *enemy_phys = new phys_obj;
    graphics_obj *shots[NUM_SHOTS];
    phys_obj *shots_phys[NUM_SHOTS];
    graphics_obj *enemy_shots[NUM_SHOTS_ENEMY];
    phys_obj *enemy_shots_phys[NUM_SHOTS_ENEMY];

    // Physics
    phys *physics = new phys(RES_X, RES_Y);

    // Ship
    ship_phys->type_id = ID_PLAYER_SHIP;
    ship_phys->pos_x = 50;
    ship_phys->pos_y = 550;
    ship_phys->size_x = 21;
    ship_phys->size_y = 21;
    ship_phys->step_x = 0;
    ship_phys->step_y = 0;
    ship_phys->move_x_every = SHIP_MOVE_PHYS_DELAY;
    ship_phys->move_y_every = SHIP_MOVE_PHYS_DELAY;
    ship_phys->move_x_last = inittime;
    ship_phys->move_y_last = inittime;
    ship_phys->bounce = 0;
    ship_phys->collided = NULL;
    ship_phys->callback = NULL;
    ship_phys->active = true;
    ship->sprite = IMG_Load("ship.png");
    ship->texture = SDL_CreateTextureFromSurface(window->renderer, ship->sprite);
    ship->pos_x = &ship_phys->pos_x;
    ship->pos_y = &ship_phys->pos_y;
    ship->active = &ship_phys->active;
    ship->size_x = 21;
    ship->size_y = 21;

    physics->add_object(ship_phys);
    window->add_object(ship);

    // Enemy
    enemy_phys->type_id = ID_ENEMY_SHIP;
    enemy_phys->pos_x = 150;
    enemy_phys->pos_y = 150;
    enemy_phys->size_x = 20;
    enemy_phys->size_y = 20;
    enemy_phys->step_x = 1;
    enemy_phys->step_y = 1;
    enemy_phys->move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    enemy_phys->move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    enemy_phys->move_x_last = inittime;
    enemy_phys->move_y_last = inittime;
    enemy_phys->bounce = 1;
    enemy_phys->collided = NULL;
    enemy_phys->callback = enemy_callback;
    enemy_phys->active = true;
    enemy->sprite = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
    enemy->pos_x = &enemy_phys->pos_x;
    enemy->pos_y = &enemy_phys->pos_y;
    enemy->active = &enemy_phys->active;
    enemy->size_x = 20;
    enemy->size_y = 20;
    SDL_FillRect(enemy->sprite, NULL, SDL_MapRGB(enemy->sprite->format, 255, 0, 0));
    enemy->texture = SDL_CreateTextureFromSurface(window->renderer, enemy->sprite);

    physics->add_object(enemy_phys);    
    window->add_object(enemy);

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        // Shots
        shots[shot_count] = new graphics_obj;
        shots_phys[shot_count] = new phys_obj;

        shots_phys[shot_count]->type_id = ID_PLAYER_SHOT;
        shots_phys[shot_count]->pos_x = ship_phys->pos_x+(ship_phys->size_x/2);
        shots_phys[shot_count]->pos_y = ship_phys->pos_y-ship_phys->size_y;
        shots_phys[shot_count]->size_x = 2;
        shots_phys[shot_count]->size_y = 10;
        shots_phys[shot_count]->step_x = 0;
        shots_phys[shot_count]->step_y = -1;
        shots_phys[shot_count]->move_x_every = SHOT_PHYS_DELAY;
        shots_phys[shot_count]->move_y_every = SHOT_PHYS_DELAY;
        shots_phys[shot_count]->move_x_last = inittime;
        shots_phys[shot_count]->move_y_last = inittime;
        shots_phys[shot_count]->bounce = -1;
        shots_phys[shot_count]->collided = NULL;
        shots_phys[shot_count]->callback = shot_callback;
        shots_phys[shot_count]->active = false;
        shots[shot_count]->sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
        shots[shot_count]->pos_x = &shots_phys[shot_count]->pos_x;
        shots[shot_count]->pos_y = &shots_phys[shot_count]->pos_y;
        shots[shot_count]->active = &shots_phys[shot_count]->active;
        shots[shot_count]->size_x = 2;
        shots[shot_count]->size_y = 10;
        SDL_FillRect(shots[shot_count]->sprite, NULL, SDL_MapRGB(shots[shot_count]->sprite->format, 0, 255, 0));
        shots[shot_count]->texture = SDL_CreateTextureFromSurface(window->renderer, shots[shot_count]->sprite);

        physics->add_object(shots_phys[shot_count]);
        window->add_object(shots[shot_count]);
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        // Shots
        enemy_shots[enemy_shot_count] = new graphics_obj;
        enemy_shots_phys[enemy_shot_count] = new phys_obj;

        enemy_shots_phys[enemy_shot_count]->type_id = ID_ENEMY_SHOT;
        enemy_shots_phys[enemy_shot_count]->pos_x = enemy_phys->pos_x+(enemy_phys->size_x/2);
        enemy_shots_phys[enemy_shot_count]->pos_y = enemy_phys->pos_y+enemy_phys->size_y+1;
        enemy_shots_phys[enemy_shot_count]->size_x = 2;
        enemy_shots_phys[enemy_shot_count]->size_y = 10;
        enemy_shots_phys[enemy_shot_count]->step_x = 0;
        enemy_shots_phys[enemy_shot_count]->step_y = 1;
        enemy_shots_phys[enemy_shot_count]->move_x_every = SHOT_PHYS_DELAY;
        enemy_shots_phys[enemy_shot_count]->move_y_every = SHOT_PHYS_DELAY;
        enemy_shots_phys[enemy_shot_count]->move_x_last = inittime;
        enemy_shots_phys[enemy_shot_count]->move_y_last = inittime;
        enemy_shots_phys[enemy_shot_count]->bounce = -1;
        enemy_shots_phys[enemy_shot_count]->collided = NULL;
        enemy_shots_phys[enemy_shot_count]->callback = shot_callback;
        enemy_shots_phys[enemy_shot_count]->active = false;
        enemy_shots[enemy_shot_count]->sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
        enemy_shots[enemy_shot_count]->pos_x = &enemy_shots_phys[enemy_shot_count]->pos_x;
        enemy_shots[enemy_shot_count]->pos_y = &enemy_shots_phys[enemy_shot_count]->pos_y;
        enemy_shots[enemy_shot_count]->active = &enemy_shots_phys[enemy_shot_count]->active;
        enemy_shots[enemy_shot_count]->size_x = 2;
        enemy_shots[enemy_shot_count]->size_y = 10;
        SDL_FillRect(enemy_shots[enemy_shot_count]->sprite, NULL, SDL_MapRGB(enemy_shots[enemy_shot_count]->sprite->format, 180, 0, 0));
        enemy_shots[enemy_shot_count]->texture = SDL_CreateTextureFromSurface(window->renderer, enemy_shots[enemy_shot_count]->sprite);

        physics->add_object(enemy_shots_phys[enemy_shot_count]);
        window->add_object(enemy_shots[enemy_shot_count]);
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
            ship_phys->step_x = 0;
            if (left) { ship_phys->step_x = -1; }
            if (right) { ship_phys->step_x = 1; }

            if (fire) {
                if (!fired) {
                    fired = true;
                    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
                        if (!shots_phys[shot_count]->active) {
                            shots_phys[shot_count]->pos_x = ship_phys->pos_x+(ship_phys->size_x/2);
                            shots_phys[shot_count]->pos_y = ship_phys->pos_y-ship_phys->size_y;
                            shots_phys[shot_count]->active = true;
                            shots_phys[shot_count]->move_x_last = inittime;
                            shots_phys[shot_count]->move_y_last = inittime;

                            break;
                        }
                    }
                }
            } else {
                fired = false;
            }

            if (reset_enemy) {
                reset_enemy = false;
                enemy_phys->pos_x = 150;
                enemy_phys->pos_y = 150;
                enemy_phys->step_x = 1;
            }

            clock_gettime(CLOCK_MONOTONIC, &now);
            timediff = ((now.tv_sec - last_enemy_shot.tv_sec) * 1000000000) + (now.tv_nsec - last_enemy_shot.tv_nsec);

            if (timediff > ENEMY_SHOT_DELAY) {
                for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
                    if (!enemy_shots_phys[enemy_shot_count]->active) {
                        enemy_shots_phys[enemy_shot_count]->pos_x = enemy_phys->pos_x+(enemy_phys->size_x/2);
                        enemy_shots_phys[enemy_shot_count]->pos_y = enemy_phys->pos_y+enemy_phys->size_y+1;
                        enemy_shots_phys[enemy_shot_count]->active = true;
                        enemy_shots_phys[enemy_shot_count]->move_x_last = inittime;
                        enemy_shots_phys[enemy_shot_count]->move_y_last = inittime;

                        break;
                    }
                }

                last_enemy_shot = now;
            }

            // Advance physics
            physics->advance();
        }

        // Redraw screen
        window->draw();
    }

    SDL_Quit();

    delete physics;
    delete window;
    delete ship_phys;
    delete ship;
    delete enemy;
    delete enemy_phys;

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        delete shots_phys[shot_count];
        delete shots[shot_count];
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        delete enemy_shots_phys[enemy_shot_count];
        delete enemy_shots[enemy_shot_count];
    }

    return;
}

int main (int argc, char *argv[])
{
    shooter();
    return 0;
}

