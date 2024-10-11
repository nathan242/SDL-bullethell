#ifndef ENEMY_DIAGONAL_STATIONARY_FWDSPRD_H
#define ENEMY_DIAGONAL_STATIONARY_FWDSPRD_H

#include "enemy_diagonal_stationary.h"

class enemy_diagonal_stationary_fwdsprd : public enemy_diagonal_stationary
{
    using enemy_diagonal_stationary::enemy_diagonal_stationary;
    
    protected:
        void init_projectile();

    public:
        void fire();
};

#endif
