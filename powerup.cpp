#include "powerup.h"

powerup::powerup(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void powerup::init()
{
    engine_obj::init();

    size_x = 21;
    size_y = 21;
    phys_size_x = 21;
    phys_size_y = 21;
    bounce = -1;

    initialized = true;
}

powerup::~powerup()
{
    if (initialized) {
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(sprite);
    }
}
