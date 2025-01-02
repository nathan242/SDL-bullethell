#include "background.h"
#include <SDL2/SDL_image.h>

extern int ID_BACKGROUND;

background::background(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void background::init()
{
    engine_obj::init();

    type_id = ID_BACKGROUND;
    size_x = 800;
    size_y = 2048;
    phys_size_x = 800;
    phys_size_y = 2048;
    area_y_offset = 0;
    step_y = 1;
    move_x_every = 0;
    move_y_every = 20000000;
    bounce = 0;
    texture = (SDL_Texture*)i_eng->get_resource("background_tex");
    phys_active = true;
    draw_active = true;
    phys_collision_active = false;

    initialized = true;
}

bool background::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    return false;
}

void background::pre_phys_event()
{
    if (pos_y > size_y) {
        pos_y = size_y*-1;
    }
}

background::~background()
{
    if (initialized) {
        
    }
}
