#include "enemy_diagonal_stationary_spiral.h"
#include <SDL2/SDL_image.h>
#include <cmath>

extern int SHOT_PHYS_DELAY;

void enemy_diagonal_stationary_spiral::init()
{
    enemy_diagonal_stationary::init();

    shot_timer = add_timer(50000000);

    default_health = 30;
    current_health = 30;
}

void enemy_diagonal_stationary_spiral::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_ball_tex");
    default_shot_texture_invincible = (SDL_Texture*)i_eng->get_resource("projectile_ball_invincible_tex");
    ball_fire_step_x = 1;
    ball_fire_step_y = 1;
    ball_shot_angle = 0;
    ball_shot_angle_diff = 10;
    shot_invincible = false;
}

void enemy_diagonal_stationary_spiral::fire()
{
    int move_shot_x_every = 5/cos(ball_shot_angle*(M_PI/180))*1000000;
    int move_shot_y_every = 5/sin(ball_shot_angle*(M_PI/180))*1000000;

    ball_shot_angle += ball_shot_angle_diff;

    if (ball_shot_angle > 90 || ball_shot_angle < 0) {
        ball_shot_angle_diff *= -1;
        ball_shot_angle += ball_shot_angle_diff*2;
        update_fire_step();
    }

    p_mngr->fire(
        shot_invincible ? default_shot_texture_invincible : default_shot_texture,
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

void enemy_diagonal_stationary_spiral::update_fire_step()
{
    if (ball_fire_step_x == 1 && ball_fire_step_y == 1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == 1) {
        ball_fire_step_y *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == -1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == 1 && ball_fire_step_y == -1) {
        ball_fire_step_y *= -1;
    }
}
