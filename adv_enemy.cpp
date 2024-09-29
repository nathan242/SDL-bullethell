#include "adv_enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;
extern int SHOT_PHYS_DELAY;

void adv_enemy::init()
{
    type_id = ID_ENEMY_SHIP;
    pos_x = 150;
    pos_y = 150;
    size_x = 40;
    size_y = 40;
    phys_size_x = 40;
    phys_size_y = 40;
    step_x = 1;
    step_y = 1;
    move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY*2;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = 1;
    collided = NULL;
    phys_active = false;
    draw_active = false;
    sprite = IMG_Load("enemy_ship_adv.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    init_projectile();

    initialized = true;
}

void adv_enemy::init_projectile()
{
    SDL_Surface *shot_sprite = IMG_Load("projectile_ball.png");
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);
}

void adv_enemy::fire()
{
    for (int x = -1; x < 2; x += 2) {
        for (int y = -1; y < 2; y += 2) {
            p_mngr->fire(
                default_shot_texture,
                20,
                20,
                20,
                20,
                pos_x+(size_x/2),
                y == 1 ? pos_y+size_y+1 : pos_y-41,
                x,
                y,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY
            );
        }
    }
}
