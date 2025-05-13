#ifndef SHIP_H
#define SHIP_H

#include "engine.h"
#include "shield.h"
#include "projectile_manager.h"

class ship : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;
        shield *shield_obj;

    public:
        int active_weapon;
        ship(engine *eng, projectile_manager *projectile_mngr);
        void init();
        void reset();
        void fire();
        void activate_shield();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        ~ship();
};

#endif