#ifndef ENEMY_PROJECTILE_H
#define ENEMY_PROJECTILE_H

#include "engine.h"

class enemy_projectile : public engine_obj
{
    public:
        enemy_projectile();
        ~enemy_projectile();
};

bool enemy_projectile_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y);

#endif
