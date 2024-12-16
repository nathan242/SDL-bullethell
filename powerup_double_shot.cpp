#include "powerup_double_shot.h"
#include <SDL2/SDL_image.h>

extern int ID_POWERUP_DOUBLE_SHOT;

void powerup_double_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_DOUBLE_SHOT;

    texture = (SDL_Texture*)i_eng->get_resource("powerup_double_shot_tex");
}
