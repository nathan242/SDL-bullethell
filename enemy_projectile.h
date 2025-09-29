#ifndef ENEMY_PROJECTILE_H
#define ENEMY_PROJECTILE_H

#include "base_projectile.h"
#include "explosion_manager.h"

class enemy_projectile : public base_projectile
{
    protected:
        engine *i_eng;
        explosion_manager *e_mngr;
        Mix_Chunk *explosion_sfx;

    public:
        enemy_projectile(engine *eng, explosion_manager *explosion_mngr);
        void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
};

#endif
