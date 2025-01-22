#ifndef ENEMY_ADV_H
#define ENEMY_ADV_H

#include "enemy.h"

class enemy_adv : public enemy
{
    using enemy::enemy;

    protected:
        int pos_mid;
        engine_obj *ship_obj;
        void pre_phys_event();

    public:
        void init();
        void fire();
};

#endif
