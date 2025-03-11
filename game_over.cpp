#include "game_over.h"
#include <SDL2/SDL_image.h>

game_over::game_over(engine *eng)
{
    i_eng = eng;
}

void game_over::init()
{
    engine_obj::init();

    size_x = 115;
    size_y = 20;
    pos_y = 290;
    pos_x = 343;
    texture = (SDL_Texture*)i_eng->get_resource("game_over_tex");
}
