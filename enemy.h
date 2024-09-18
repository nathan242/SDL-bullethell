#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"
#include "projectile_manager.h"

class enemy : public engine_obj
{
    protected:
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;

    public:
        enemy(engine *eng, projectile_manager *projectile_mngr);
        void fire();
        ~enemy();
};

void enemy_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y);

#endif
