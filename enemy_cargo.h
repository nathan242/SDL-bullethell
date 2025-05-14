#ifndef ENEMY_CARGO_H
#define ENEMY_CARGO_H

#include "base_enemy.h"
#include "powerup.h"

class enemy_cargo : public base_enemy
{
    using base_enemy::base_enemy;

    public:
        void init();
};

#endif
