#ifndef ENEMY_DIAGONAL_STATIONARY_H
#define ENEMY_DIAGONAL_STATIONARY_H

#include "enemy.h"

class enemy_diagonal_stationary : public enemy
{
    using enemy::enemy;
    
    protected:
        void init_projectile();
        void pre_phys_event();

    public:
        void init();
        void fire();
};

#endif
