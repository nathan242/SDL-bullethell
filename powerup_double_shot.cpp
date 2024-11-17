#include "powerup_double_shot.h"
#include <SDL2/SDL_image.h>

extern int ID_POWERUP_DOUBLE_SHOT;

void powerup_double_shot::init()
{
    powerup::init();

    type_id = ID_POWERUP_DOUBLE_SHOT;

    sprite = IMG_Load("powerup_double_shot.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);
}
