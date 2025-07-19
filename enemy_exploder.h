#ifndef ENEMY_EXPLODER_H
#define ENEMY_EXPLODER_H

#include "base_enemy.h"

class enemy_exploder : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        animation_obj *invincible_shot_animation;
        void init_projectile();
        void post_destroy();

    public:
        void init();
        void fire();
        ~enemy_exploder();
};

#endif
