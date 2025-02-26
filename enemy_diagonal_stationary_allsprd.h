#ifndef ENEMY_DIAGONAL_STATIONARY_ALLSPRD_H
#define ENEMY_DIAGONAL_STATIONARY_ALLSPRD_H

#include "enemy_diagonal_stationary.h"

class enemy_diagonal_stationary_allsprd : public enemy_diagonal_stationary
{
    using enemy_diagonal_stationary::enemy_diagonal_stationary;

    protected:
        int ball_fire_step_x;
        int ball_fire_step_y;
        int ball_shot_angle;
        int ball_shot_angle_diff;
        bool shot_invincible;
        SDL_Texture *default_shot_texture_invincible;
        void init_projectile();
        bool update_fire_step();

    public:
        void init();
        void fire();
};

#endif
