#ifndef SHIELD_H
#define SHIELD_H

#include "engine.h"

class shield : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        timer_obj *activation_timer;
        engine_obj *target;
        int target_offset_x;
        int target_offset_y;

    public:
        shield(engine *eng);
        void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        void activate(engine_obj *target_obj, int offset_x = 0, int offset_y = 0);
        ~shield();
};

#endif
