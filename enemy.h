#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"
#include "projectile_manager.h"

class enemy : public engine_obj
{
    protected:
        engine *i_eng;
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;

        virtual void init_projectile();

    public:
        enemy(engine *eng, projectile_manager *projectile_mngr);
        virtual void init();
        virtual void fire();
        ~enemy();
};

bool enemy_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y);

#endif
