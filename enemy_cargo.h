#ifndef ENEMY_CARGO_H
#define ENEMY_CARGO_H

#include "enemy.h"
#include "powerup.h"

class enemy_cargo : public enemy
{
    using enemy::enemy;

    public:
        powerup *drop_powerup;
        void init();
        void damage(int damage_amount);
};

#endif
