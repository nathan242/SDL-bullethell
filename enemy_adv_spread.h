#ifndef ENEMY_ADV_SPREAD_H
#define ENEMY_ADV_SPREAD_H

#include "base_enemy.h"

class enemy_adv_spread : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        void pre_phys_event();
        void init_projectile();
        int pos_mid;

    public:
        void init();
        void fire();
};

#endif
