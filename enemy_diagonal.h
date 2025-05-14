#ifndef ENEMY_DIAGONAL_H
#define ENEMY_DIAGONAL_H

#include "base_enemy.h"

class enemy_diagonal : public base_enemy
{
    using base_enemy::base_enemy;
    
    protected:
        void init_projectile();

    public:
        void init();
        void fire();
};

#endif
