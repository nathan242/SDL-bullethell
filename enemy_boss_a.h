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
        uint64_t ball_shot_delay;
        timespec ball_last_shot;
        engine_obj *ship_obj;
        SDL_Texture *ball_shot_texture;
        SDL_Texture *ball_invincible_shot_texture;
        void update_fire_step();
        void init_projectile();
        void pre_phys_event();

    public:
        void init();
        void ball_fire();
        void fire();
};

#endif
