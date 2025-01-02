#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "engine.h"

class background : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;

    public:
        background(engine *eng);
        virtual void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        ~background();
};

#endif
