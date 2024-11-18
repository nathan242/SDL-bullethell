#include "powerup_quad_spread_shot.h"
#include <SDL2/SDL_image.h>

extern int ID_POWERUP_QUAD_SPREAD_SHOT;

void powerup_quad_spread_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_QUAD_SPREAD_SHOT;

    sprite = IMG_Load("powerup_quad_spread_shot.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);
}
