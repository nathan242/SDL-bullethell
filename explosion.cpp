#include "explosion.h"
#include <SDL2/SDL_image.h>

explosion::explosion(engine *eng)
{
    i_eng = eng;
}

void explosion::init()
{
    engine_obj::init();

    draw_active = true;
}

explosion::~explosion()
{
    if (animation != NULL) {
        delete animation;
    }
}
