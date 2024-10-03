#ifndef ENEMY_DIAGONAL_H
#define ENEMY_DIAGONAL_H

#include "enemy.h"

class enemy_diagonal : public enemy
{
    using enemy::enemy;
    
    protected:
        void init_projectile();

    public:
        void init();
        void fire();
};

#endif
