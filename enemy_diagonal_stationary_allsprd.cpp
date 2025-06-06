#include "enemy_diagonal_stationary_allsprd.h"
#include <SDL2/SDL_image.h>
#include <cmath>

extern int SHOT_PHYS_DELAY;

void enemy_diagonal_stationary_allsprd::init()
{
    enemy_diagonal_stationary::init();

    shot_timer = add_timer(1000000000);

    default_health = 40;
    current_health = 40;
}

void enemy_diagonal_stationary_allsprd::init_projectile()
{
    default_shot_texture = NULL;
    default_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
    default_shot_invincible_animation = (animation_obj*)i_eng->get_resource("projectile_ball_invincible_anim");
    ball_fire_step_x = 1;
    ball_fire_step_y = 1;
    ball_shot_angle = 0;
    ball_shot_angle_diff = 10;
    shot_invincible = false;
}

void enemy_diagonal_stationary_allsprd::fire()
{
    int move_shot_x_every;
    int move_shot_y_every;
    bool run_loop = true;

    while (run_loop) {
        move_shot_x_every = 9/cos(ball_shot_angle*(M_PI/180))*1000000;
        move_shot_y_every = 9/sin(ball_shot_angle*(M_PI/180))*1000000;

        ball_shot_angle += ball_shot_angle_diff;

        if (ball_shot_angle > 90 || ball_shot_angle < 0) {
            ball_shot_angle_diff *= -1;
            ball_shot_angle += ball_shot_angle_diff*2;
            run_loop = update_fire_step();
        }

        p_mngr->fire(
            shot_invincible ? default_shot_invincible_animation : default_shot_animation,
            20,
            20,
            20,
            20,
            pos_x+(size_x/2)-10,
            pos_y+(size_y/2)-10,
            ball_fire_step_x,
            ball_fire_step_y,
            move_shot_x_every,
            move_shot_y_every,
            shot_invincible
        );

        shot_invincible = !shot_invincible;
    }
}

bool enemy_diagonal_stationary_allsprd::update_fire_step()
{
    if (ball_fire_step_x == 1 && ball_fire_step_y == 1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == 1) {
        ball_fire_step_y *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == -1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == 1 && ball_fire_step_y == -1) {
        ball_fire_step_y *= -1;
        return false;
    }

    return true;
}
