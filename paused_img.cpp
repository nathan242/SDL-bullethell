#include "paused_img.h"
#include <SDL2/SDL_image.h>

paused_img::paused_img(engine *eng)
{
    i_eng = eng;
}

void paused_img::init()
{
    engine_obj::init();

    size_x = 115;
    size_y = 20;
    pos_y = 290;
    pos_x = 343;
    texture = (SDL_Texture*)i_eng->get_resource("paused_tex");
}
