#include "enemy_boss_a.h"
#include <SDL2/SDL_image.h>
#include <cmath>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY_DEF 10000000
#define SHOT_PHYS_DELAY_DIFF 20000000
#define SHOT_PHYS_DELAY_DIFF_START 100000000

extern int ID_ENEMY_SHIP;
extern int SHOT_PHYS_DELAY;
extern int HIT_FLASH_DELAY;

void enemy_boss_a::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 100;
    size_y = 100;
    phys_size_x = 100;
    phys_size_y = 100;
    area_y_offset = 100;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 200;
    current_health = 200;

    shot_timer = add_timer(400000000);
    ball_shot_timer = add_timer(50000000);

    drop_powerup = NULL;
    ungroup = false;
    ungroup_at_y = 0;

    ship_obj = (engine_obj*)i_eng->get_resource("ship_obj");

    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    init_projectile();

    initialized = true;
}

void enemy_boss_a::init_projectile()
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

void enemy_boss_a::pre_phys_event()
{
    if (pos_y >= 200) {
        step_y = 0;
    }

    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    if (pos_y > (size_y * -1) && ball_shot_timer->tick(i_eng->timer_now)) {
        ball_fire();
    }

    enemy::pre_phys_event();
}

void enemy_boss_a::ball_fire()
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
        shot_invincible ? ball_invincible_shot_animation : ball_shot_animation,
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

void enemy_boss_a::fire()
{
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-6,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+6,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-18,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+18,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );
}

void enemy_boss_a::update_fire_step()
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
