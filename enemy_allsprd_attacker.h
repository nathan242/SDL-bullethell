#ifndef ENEMY_ALLSPRD_ATTACKER_H
#define ENEMY_ALLSPRD_ATTACKER_H

#include "base_enemy.h"

class enemy_allsprd_attacker : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        int ball_fire_step_x;
        int ball_fire_step_y;
        int ball_shot_angle;
        int ball_shot_angle_diff;
        bool shot_invincible;
        animation_obj *default_shot_invincible_animation;
        animation_obj *invincible_shot_animation;
        void init_projectile();
        void post_destroy();
        bool update_fire_step();

    public:
        void init();
        void fire();
        ~enemy_allsprd_attacker();
};

#endif
