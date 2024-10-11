#include "enemy_diagonal_stationary.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;

void enemy_diagonal_stationary::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 40;
    size_y = 40;
    phys_size_x = 40;
    phys_size_y = 40;
    area_y_offset = 40;
    move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY*2;
    bounce = 1;
    sprite = IMG_Load("enemy_ship_diagonal.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    init_projectile();

    initialized = true;
}

void enemy_diagonal_stationary::init_projectile()
{
    SDL_Surface *shot_sprite = IMG_Load("projectile_ball.png");
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);
}

void enemy_diagonal_stationary::pre_phys_event()
{
    if (pos_y >= 200) {
        step_y = 0;
    }

    enemy::pre_phys_event();
}

void enemy_diagonal_stationary::fire()
{
    int fire_pos_x;
    int fire_pos_y;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            fire_pos_x = x == 1 ? pos_x+size_x : pos_x-20;
            fire_pos_y = y == 1 ? pos_y+size_y : pos_y-20;

            if (x == 0 && y == 0) {
                continue;
            } else if (x == 0) {
                fire_pos_x = pos_x+(size_x/2)-10;
            } else if (y == 0) {
                fire_pos_y = pos_y+(size_y/2)-10;
            }

            p_mngr->fire(
                default_shot_texture,
                20,
                20,
                20,
                20,
                fire_pos_x,
                fire_pos_y,
                x,
                y,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY,
                false
            );
        }
    }
}
