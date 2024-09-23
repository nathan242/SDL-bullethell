#ifndef PLAYER_PROJECTILE_H
#define PLAYER_PROJECTILE_H

#include "engine.h"

class player_projectile : public engine_obj
{
    public:
        void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
};

#endif
