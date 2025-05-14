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
        timer_obj *shield_charge_timer;

    public:
        int active_weapon;
        int shield_charge_level;
        ship(engine *eng, projectile_manager *projectile_mngr);
        void init();
        void reset();
        void fire();
        void activate_shield();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        ~ship();
};

#endif