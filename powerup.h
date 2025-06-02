#ifndef POWERUP_H
#define POWERUP_H

#include "engine.h"

class powerup : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;

    public:
        powerup(engine *eng);
        virtual void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        ~powerup();
};

#endif
