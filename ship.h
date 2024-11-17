#ifndef SHIP_H
#define SHIP_H

#include "engine.h"
#include "projectile_manager.h"

class ship : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;
        int active_weapon;

    public:
        ship(engine *eng, projectile_manager *projectile_mngr);
        void init();
        void fire();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        ~ship();
};

#endif