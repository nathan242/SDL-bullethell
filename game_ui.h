#ifndef GAME_UI_H
#define GAME_UI_H

#include "engine.h"
#include "ship.h"
#include "base_enemy.h"
#include <vector>

class game_ui : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        ship *ship_obj;
        std::vector<SDL_Texture*> weapons;
        SDL_Rect offset;
        SDL_Texture *boss_health;
        SDL_Texture *green_bar;
        SDL_Texture *red_bar;
        engine_obj *boss_health_target;

    public:
        game_ui(engine *eng);
        void init();
        void post_draw_event();
        void activate_boss_health(engine_obj *target_obj);
        ~game_ui();
};

#endif