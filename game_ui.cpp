#include "game_ui.h"
#include <SDL2/SDL_image.h>

game_ui::game_ui(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void game_ui::init()
{
    engine_obj::init();

    size_x = 800;
    size_y = 30;
    texture = (SDL_Texture*)i_eng->get_resource("game_ui_bar_tex");
    ship_obj = (ship*)i_eng->get_resource("ship_obj");
    weapons.push_back((SDL_Texture*)i_eng->get_resource("powerup_single_shot_tex"));
    weapons.push_back((SDL_Texture*)i_eng->get_resource("powerup_double_shot_tex"));
    weapons.push_back((SDL_Texture*)i_eng->get_resource("powerup_quad_spread_shot_tex"));

    initialized = true;
}

void game_ui::post_draw_event()
{
    offset.x = 102;
    offset.y = 4;
    offset.w = 21;
    offset.h = 21;

    SDL_RenderCopy(i_eng->renderer, weapons[ship_obj->active_weapon], NULL, &offset);
}

game_ui::~game_ui()
{
    if (initialized) {

    }
}
