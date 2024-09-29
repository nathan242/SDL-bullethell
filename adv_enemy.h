#ifndef ADV_ENEMY_H
#define ADV_ENEMY_H

#include "enemy.h"

class adv_enemy : public enemy
{
    using enemy::enemy;
    
    protected:
        void init_projectile();

    public:
        void init();
        void fire();
};

#endif
