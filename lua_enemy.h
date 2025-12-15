#ifndef LUA_ENEMY_H
#define LUA_ENEMY_H

#include "base_enemy.h"

class lua_enemy : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        void pre_phys_event();
        void init_projectile();
        const char *lua_obj_name;
        int lua_obj_ref;

    public:
        void lua_init(const char *name);
        void default_pre_phys_event();
        engine* get_engine();
        projectile_manager* get_p_mngr();
        void set_shot_timer(timer_obj *timer);
        void set_default_texture(SDL_Texture *texture);
        void set_hit_texture(SDL_Texture *texture);
        void set_shot_texture(SDL_Texture *texture);
        SDL_Texture* get_shot_texture();
        void set_default_shot_sfx(Mix_Chunk *sfx);
        void init();
        void fire();
        ~lua_enemy();
};

#endif
