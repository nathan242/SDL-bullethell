#include "paused_img.h"

paused_img::paused_img(engine *eng)
{
    i_eng = eng;
}

void paused_img::init()
{
    engine_obj::init();

    size_x = 115;
    size_y = 20;
    pos_y = (i_eng->area_y / 2) - 10;
    pos_x = (i_eng->area_x / 2) - 57;
    texture = (SDL_Texture*)i_eng->get_resource("paused_tex");
}
