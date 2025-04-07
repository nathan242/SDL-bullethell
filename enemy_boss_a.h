#ifndef ENEMY_BOSS_A_H
#define ENEMY_BOSS_A_H

#include "enemy.h"

class enemy_boss_a : public enemy
{
    using enemy::enemy;

    protected:
        int ball_fire_step_x;
        int ball_fire_step_y;
        int ball_shot_angle;
        int ball_shot_angle_diff;
        int pos_mid;
        bool shot_invincible;
        timer_obj *ball_shot_timer;
        engine_obj *ship_obj;
        animation_obj *ball_shot_animation;
        animation_obj *ball_invincible_shot_animation;
        void update_fire_step();
        void init_projectile();
        void pre_phys_event();

    public:
        void init();
        void ball_fire();
        void fire();
};

#endif
