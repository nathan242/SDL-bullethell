#include "anim_explosion.h"
#include <SDL2/SDL_image.h>

anim_explosion::anim_explosion(timer_obj *timer_object, engine *eng)
{
    timer = timer_object;
    curr = 0;

    timer->every = 100000000;

    frames.push_back(NULL);
    frames.push_back((SDL_Texture*)eng->get_resource("explosion_1_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("explosion_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("explosion_3_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("explosion_4_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("explosion_1_tex"));
}
