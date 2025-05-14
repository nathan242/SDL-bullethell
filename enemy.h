#ifndef ENEMY_H
#define ENEMY_H

#include "base_enemy.h"

class enemy : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        virtual void init_projectile();

    public:
        void init();
        void fire();
        ~enemy();
};

#endif
