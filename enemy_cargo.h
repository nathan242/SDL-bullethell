#ifndef ENEMY_CARGO_H
#define ENEMY_CARGO_H

#include "enemy.h"

class enemy_cargo : public enemy
{
    using enemy::enemy;

    public:
        void init();
};

#endif
