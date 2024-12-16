#include "powerup_quad_spread_shot.h"
#include <SDL2/SDL_image.h>

extern int ID_POWERUP_QUAD_SPREAD_SHOT;

void powerup_quad_spread_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_QUAD_SPREAD_SHOT;

    texture = (SDL_Texture*)i_eng->get_resource("powerup_quad_spread_shot_tex");
}
