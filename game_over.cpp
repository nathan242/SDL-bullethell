#include "game_over.h"

game_over::game_over(engine *eng)
{
    i_eng = eng;
}

void game_over::init()
{
    engine_obj::init();

    size_x = 115;
    size_y = 20;
    pos_y = (i_eng->area_y / 2) -10;
    pos_x = (i_eng->area_x / 2) - 57;
    texture = (SDL_Texture*)i_eng->get_resource("game_over_tex");
}
