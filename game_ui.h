#ifndef GAME_UI_H
#define GAME_UI_H

#include "engine.h"
#include "ship.h"
#include <vector>

class game_ui : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        ship *ship_obj;
        std::vector<SDL_Texture*> weapons;
        SDL_Rect offset;

    public:
        game_ui(engine *eng);
        void init();
        void post_draw_event();
        ~game_ui();
};

#endif