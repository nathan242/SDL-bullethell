#include "background.h"
#include "constants.h"

background::background(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void background::init(int window_res_y)
{
    engine_obj::init();

    res_y = window_res_y;

    type_id = ID_BACKGROUND;
    step_y = 1;
    move_x = add_timer(0);
    move_y = add_timer(BACKGROUND_SCROLL_EVERY);
    bounce = 0;
    phys_active = true;
    draw_active = true;
    phys_collision_active = false;

    initialized = true;
}

void background::set(SDL_Texture *background_texture, int background_size_x, int background_size_y)
{
    texture = background_texture;
    size_x = phys_size_x = background_size_x;
    size_y = phys_size_y = background_size_y;

    pos_x = 0;
    pos_y = 0;
}

bool background::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    return false;
}

void background::pre_phys_event()
{
    if (pos_y >= res_y) {
        pos_y = (size_y*-1) + res_y;
    }
}

void background::post_draw_event()
{
    offset.x = pos_x;
    offset.w = size_x;
    offset.h = size_y;

    offset.y = pos_y >= 0 ? pos_y - size_y : pos_y + size_y;

    SDL_RenderCopy(i_eng->renderer, texture, NULL, &offset);
}

background::~background()
{
    if (initialized) {
        
    }
}
