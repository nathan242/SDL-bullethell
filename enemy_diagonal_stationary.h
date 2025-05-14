#ifndef ENEMY_DIAGONAL_STATIONARY_H
#define ENEMY_DIAGONAL_STATIONARY_H

#include "base_enemy.h"

class enemy_diagonal_stationary : public base_enemy
{
    using base_enemy::base_enemy;
    
    protected:
        void init_projectile();
        void pre_phys_event();

    public:
        virtual void init();
        virtual void fire();
};

#endif
