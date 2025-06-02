#ifndef ANIM_EXPLOSION_H
#define ANIM_EXPLOSION_H

#include "engine.h"

class anim_explosion : public animation_obj
{
    public:
        anim_explosion(timer_obj *timer_object, engine *eng);
};

#endif
