#ifndef ENEMY_H
#define ENEMY_H

#include "engine.h"
#include "projectile_manager.h"
#include "powerup.h"

class enemy : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        projectile_manager *p_mngr;
        SDL_Texture *default_shot_texture;
        timer_obj *shot_timer;
        int default_health;
        int current_health;
        SDL_Texture *default_texture;
        SDL_Texture *hit_texture;
        timer_obj *last_hit_timer;

        virtual void init_projectile();

    public:
        powerup *drop_powerup;
        bool ungroup;
        int ungroup_at_y;

        enemy(engine *eng, projectile_manager *projectile_mngr);
        virtual void init();
        virtual void fire();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        virtual void damage(int damage_amount);
        ~enemy();
};

#endif
