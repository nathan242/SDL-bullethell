#include "enemy_boss_b.h"
#include <cmath>
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY_DEF 10000000
#define SHOT_PHYS_DELAY_DIFF 20000000
#define SHOT_PHYS_DELAY_DIFF_START 100000000

void enemy_boss_b::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 150;
    size_y = 215;
    phys_size_x = 150;
    phys_size_y = 215;
    area_y_offset = 150;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_boss_b_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_boss_b_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 150;
    current_health = 150;

    shot_timer = add_timer(700000000);
    side_shot_timer = add_timer(900000000);
    far_side_shot_timer = add_timer(1100000000);
    ball_shot_timer_front = add_timer(1800000000);
    ball_shot_timer_back = add_timer(2000000000);

    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    game_ui_obj = (game_ui*)i_eng->get_resource("game_ui_obj");
    game_ui_obj->activate_boss_health(this);

    initialized = true;
}

void enemy_boss_b::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
    default_shot_animation = NULL;
    ball_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
    ball_invincible_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_invincible_anim");
    ball_fire_step_x = 1;
    ball_fire_step_y = 1;
    ball_shot_angle = 0;
    ball_shot_angle_diff = 10;
    shot_invincible = false;
}

void enemy_boss_b::pre_phys_event()
{
    if (pos_y >= 100) {
        step_y = 0;
    }

    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    if (pos_y > (size_y * -1)) {
        if (ball_shot_timer_front->tick(i_eng->timer_now)) {
            ball_fire(40);
        }

        if (ball_shot_timer_back->tick(i_eng->timer_now)) {
            ball_fire(-40);
        }

        if (side_shot_timer->tick(i_eng->timer_now)) {
            fire_side();
        }

        if (far_side_shot_timer->tick(i_eng->timer_now)) {
            fire_far_side();
        }
    }

    base_enemy::pre_phys_event();
}

void enemy_boss_b::ball_fire(int offset_y)
{
    int move_shot_x_every;
    int move_shot_y_every;
    bool run_loop = true;

    while (run_loop) {
        move_shot_x_every = 9/cos(ball_shot_angle*(M_PI/180))*800000;
        move_shot_y_every = 9/sin(ball_shot_angle*(M_PI/180))*800000;

        ball_shot_angle += ball_shot_angle_diff;

        if (ball_shot_angle > 90 || ball_shot_angle < 0) {
            ball_shot_angle_diff *= -1;
            ball_shot_angle += ball_shot_angle_diff*2;
            run_loop = update_fire_step();
        }

        p_mngr->fire(
            shot_invincible ? ball_invincible_shot_animation : ball_shot_animation,
            10,
            10,
            10,
            10,
            pos_x+(size_x/2)-5,
            pos_y+(size_y/2)-5+offset_y,
            ball_fire_step_x,
            ball_fire_step_y,
            move_shot_x_every,
            move_shot_y_every,
            shot_invincible
        );

        shot_invincible = !shot_invincible;
    }
}

bool enemy_boss_b::update_fire_step()
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

void enemy_boss_b::fire()
{
    // Front left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-7,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+2,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );
}

void enemy_boss_b::fire_side()
{
    // Front left ball
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)-22,
        pos_y+size_y-14,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front right ball
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)+12,
        pos_y+size_y-14,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );
}

void enemy_boss_b::fire_far_side()
{
    // Front far left right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-53,
        pos_y+size_y-71,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front far left left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-60,
        pos_y+size_y-71,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front far right left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+48,
        pos_y+size_y-71,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front far right right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+55,
        pos_y+size_y-71,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );
}
