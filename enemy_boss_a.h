#ifndef ENEMY_BOSS_A_H
#define ENEMY_BOSS_A_H

#include "enemy.h"

class enemy_boss_a : public enemy
{
    using enemy::enemy;

    protected:
        int move_shot_x_every;
        int move_shot_y_every;
        int *move_shot_a;
        int *move_shot_b;
        int diff_shot_direction;
        int fire_step_x;
        int fire_step_y;
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
