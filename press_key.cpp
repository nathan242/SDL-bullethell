#include "press_key.h"
#include <SDL2/SDL_image.h>

press_key::press_key(engine *eng)
{
    i_eng = eng;
}

void press_key::init()
{
    engine_obj::init();

    size_x = 212;
    size_y = 20;
    pos_y = 300;
    pos_x = 294;
    texture = (SDL_Texture*)i_eng->get_resource("press_key_tex");
}
