#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "engine.h"

class player_projectile : public engine_obj
{
    public:
        player_projectile();
        ~player_projectile();
};

void player_projectile_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y);

#endif
