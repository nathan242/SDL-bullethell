#include "press_key.h"

press_key::press_key(engine *eng)
{
    i_eng = eng;
}

void press_key::init()
{
    engine_obj::init();

    size_x = 212;
    size_y = 20;
    pos_y = (i_eng->area_y / 2) + 150;
    pos_x = (i_eng->area_x / 2) - 106;
    texture = (SDL_Texture*)i_eng->get_resource("press_key_tex");
}
