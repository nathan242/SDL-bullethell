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
    pos_y = 290;
    pos_x = 340;
    texture = (SDL_Texture*)i_eng->get_resource("complete_tex");
}
