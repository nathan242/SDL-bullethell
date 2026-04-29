#include "menu_title.h"

menu_title::menu_title(engine *eng)
{
    i_eng = eng;
}

void menu_title::init()
{
    engine_obj::init();

    size_x = 726;
    size_y = 600;
    pos_x = (i_eng->area_x / 2) - 363;
    pos_y = (i_eng->area_y / 2) - 300;
    texture = (SDL_Texture*)i_eng->get_resource("menu_title_tex");
}
