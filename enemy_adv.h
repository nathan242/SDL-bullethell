#ifndef ENEMY_ADV_H
#define ENEMY_ADV_H

#include "base_enemy.h"

class enemy_adv : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        void pre_phys_event();
        void init_projectile();

    public:
        void init();
        void fire();
};

#endif
