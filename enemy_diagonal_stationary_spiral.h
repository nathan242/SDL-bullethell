#ifndef ENEMY_DIAGONAL_STATIONARY_SPIRAL_H
#define ENEMY_DIAGONAL_STATIONARY_SPIRAL_H

#include "enemy_diagonal_stationary.h"

class enemy_diagonal_stationary_spiral : public enemy_diagonal_stationary
{
    using enemy_diagonal_stationary::enemy_diagonal_stationary;

    protected:
        int ball_fire_step_x;
        int ball_fire_step_y;
        int ball_shot_angle;
        int ball_shot_angle_diff;
        bool shot_invincible;
        animation_obj *default_shot_invincible_animation;
        void init_projectile();
        void update_fire_step();

    public:
        void init();
        void fire();
};

#endif
