#include "adv_enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;
extern int SHOT_PHYS_DELAY;

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
                y == 1 ? pos_y+size_y+1 : pos_y-10,
                x,
                y,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY
            );
        }
    }
}
