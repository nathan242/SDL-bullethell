#ifndef LEVEL_TEXT_H
#define LEVEL_TEXT_H

#include "engine.h"
#include "constants.h"

class level_text : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        SDL_Texture *numbers[NUM_LEVELS];
        timer_obj *wait_timer;
        SDL_Rect offset;
        int step;
        int show_number;

    public:
        level_text(engine *eng);
        void init();
        void set(int level);
        void pre_phys_event();
        void post_draw_event();
        ~level_text();
};

#endif
