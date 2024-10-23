#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"
#include "projectile_manager.h"

class enemy : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;
        timespec last_shot;
        int default_health;
        int current_health;
        SDL_Texture *default_texture;
        SDL_Texture *hit_texture;
        timespec last_hit;

        virtual void init_projectile();

    public:
        enemy(engine *eng, projectile_manager *projectile_mngr);
        virtual void init();
        virtual void fire();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        void damage(int damage_amount);
        ~enemy();
};

#endif
