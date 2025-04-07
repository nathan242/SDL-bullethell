#include "anim_projectile_ball_invincible.h"
#include <SDL2/SDL_image.h>

anim_projectile_ball_invincible::anim_projectile_ball_invincible(timer_obj *timer_object, engine *eng)
{
    timer = timer_object;
    curr = 0;

    timer->every = 250000000;

    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_1_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_3_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_4_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_5_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_4_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_3_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_2_tex"));
    frames.push_back((SDL_Texture*)eng->get_resource("projectile_ball_invincible_frame_1_tex"));
}
