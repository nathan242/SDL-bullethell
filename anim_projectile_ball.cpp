#include "anim_projectile_ball.h"
#include <SDL2/SDL_image.h>

anim_projectile_ball::anim_projectile_ball(timer_obj *timer_object, engine *eng)
{
    timer = timer_object;
    curr = 0;

    timer->every = 250000000;

    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_1_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_3_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_4_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_5_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_4_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_3_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_frame_1_tex"));
}
