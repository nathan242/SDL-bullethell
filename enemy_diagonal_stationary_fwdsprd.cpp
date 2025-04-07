#include "enemy_diagonal_stationary_fwdsprd.h"
#include <SDL2/SDL_image.h>

#define SHOT_PHYS_DELAY 5000000

void enemy_diagonal_stationary_fwdsprd::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_ball_invincible_tex");
    default_shot_animation = NULL;
}

void enemy_diagonal_stationary_fwdsprd::fire()
{
    for (int x = -1; x < 2; x++) {
        p_mngr->fire(
            default_shot_texture,
            20,
            20,
            20,
            20,
            pos_x+(size_x/2)+((size_x/2)*x)-10,
            pos_y+size_y,
            x,
            1,
            SHOT_PHYS_DELAY,
            SHOT_PHYS_DELAY,
            true
        );
    }
}
