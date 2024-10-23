#include "enemy_diagonal.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;

void enemy_diagonal::init()
{
    SDL_Surface *hit_sprite;

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
    last_shot = {0, 0};
    default_texture = texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    hit_sprite = IMG_Load("enemy_ship_diagonal_hit.png");
    hit_texture = SDL_CreateTextureFromSurface(i_eng->renderer, hit_sprite);
    SDL_FreeSurface(hit_sprite);

    last_hit = {0, 0};

    default_health = 4;
    current_health = 4;

    init_projectile();

    initialized = true;
}

void enemy_diagonal::init_projectile()
{
    SDL_Surface *shot_sprite = IMG_Load("projectile_ball.png");
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);
}

void enemy_diagonal::fire()
{
    for (int x = -1; x < 2; x += 2) {
        for (int y = -1; y < 2; y += 2) {
            p_mngr->fire(
                default_shot_texture,
                20,
                20,
                20,
                20,
                x == 1 ? pos_x+size_x : pos_x-20,
                y == 1 ? pos_y+size_y : pos_y-20,
                x,
                y,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY,
                false
            );
        }
    }
}
