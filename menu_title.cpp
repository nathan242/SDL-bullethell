#include "menu_title.h"
#include <SDL2/SDL_image.h>

menu_title::menu_title(engine *eng)
{
    i_eng = eng;
}

void menu_title::init()
{
    engine_obj::init();

    size_x = 800;
    size_y = 109;
    pos_y = 100;
    texture = (SDL_Texture*)i_eng->get_resource("menu_title_tex");
}
