#ifndef BASE_PROJECTILE_H
#define BASE_PROJECTILE_H

#include "engine.h"

class base_projectile : public engine_obj
{
    public:
        bool invincible;

        void init();
};

#endif
