#include "anim_powerup_quad_spread_shot.h"
#include <SDL2/SDL_image.h>

anim_powerup_quad_spread_shot::anim_powerup_quad_spread_shot(timer_obj *timer_object, engine *eng)
{
    timer = timer_object;
    curr = 0;

    timer->every = 100000000;

    frames.push_back((SDL_Texture*)eng->get_resource("powerup_quad_spread_shot_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("powerup_quad_spread_shot_frame_1_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("powerup_quad_spread_shot_frame_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("powerup_quad_spread_shot_frame_1_tex"));
}
