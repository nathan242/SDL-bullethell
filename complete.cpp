#include "complete.h"

complete::complete(engine *eng)
{
    i_eng = eng;
}

void complete::init()
{
    engine_obj::init();

    size_x = 120;
    size_y = 20;
    pos_y = (i_eng->area_y / 2) - 10;
    pos_x = (i_eng->area_x / 2) - 60;
    texture = (SDL_Texture*)i_eng->get_resource("complete_tex");
}
