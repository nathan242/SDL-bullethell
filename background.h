#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "engine.h"

class background : public engine_obj
{
    protected:
        bool initialized;
        engine *i_eng;
        SDL_Rect offset;
        int res_y;

    public:
        background(engine *eng);
        void init(int window_res_y);
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void pre_phys_event();
        void post_draw_event();
        void set(SDL_Texture *background_texture, int background_size_x, int background_size_y);
        ~background();
};

#endif
