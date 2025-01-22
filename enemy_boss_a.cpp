#include "enemy_boss_a.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY_DEF 10000000
#define SHOT_PHYS_DELAY_DIFF 20000000
#define SHOT_PHYS_DELAY_DIFF_START 100000000

extern int ID_ENEMY_SHIP;
extern int SHOT_PHYS_DELAY;

void enemy_boss_a::init()
{
    SDL_Surface *hit_sprite;

    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 100;
    size_y = 100;
    phys_size_x = 100;
    phys_size_y = 100;
    area_y_offset = 100;
    move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY*2;
    bounce = 1;
    last_shot = {0, 0};
    ball_last_shot = {0, 0};
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_hit_tex");

    last_hit = {0, 0};

    default_health = 400;
    current_health = 400;

    shot_delay = 400000000;
    ball_shot_delay = 50000000;

    drop_powerup = NULL;
    ungroup = false;

    ship_obj = (engine_obj*)i_eng->get_resource("ship_obj");

    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    init_projectile();

    initialized = true;
}

void enemy_boss_a::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
    ball_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_ball_tex");
    ball_invincible_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_ball_invincible_tex");
    move_shot_a = &move_shot_x_every;
    move_shot_b = &move_shot_y_every;
    *move_shot_a = SHOT_PHYS_DELAY_DIFF_START;
    *move_shot_b = SHOT_PHYS_DELAY;
    diff_shot_direction = 0;
    fire_step_x = 1;
    fire_step_y = 1;
    shot_invincible = false;
}

void enemy_boss_a::pre_phys_event()
{
    timespec now;
    uint64_t timediff;

    if (pos_y >= 200) {
        step_y = 0;
    }

    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    clock_gettime(CLOCK_MONOTONIC, &now);
    timediff = ((now.tv_sec - ball_last_shot.tv_sec) * 1000000000) + (now.tv_nsec - ball_last_shot.tv_nsec);

    if (ball_shot_delay > 0 && timediff > ball_shot_delay && pos_y > (size_y * -1)) {
        ball_fire();
        ball_last_shot = now;
    }

    enemy::pre_phys_event();
}

void enemy_boss_a::ball_fire()
{
    if (diff_shot_direction == 0) {
        *move_shot_a -= SHOT_PHYS_DELAY_DIFF;
    } else {
        *move_shot_b += SHOT_PHYS_DELAY_DIFF;
    }

    if (diff_shot_direction == 0 && *move_shot_a <= SHOT_PHYS_DELAY_DEF) {
        *move_shot_a = SHOT_PHYS_DELAY_DEF;
        diff_shot_direction = 1;
    } else if (diff_shot_direction == 1 && *move_shot_b >= SHOT_PHYS_DELAY_DIFF_START) {
        *move_shot_b = SHOT_PHYS_DELAY_DEF;
        *move_shot_a = SHOT_PHYS_DELAY_DIFF_START;
        diff_shot_direction = 0;
        update_fire_step();
    }

    p_mngr->fire(
        shot_invincible ? ball_invincible_shot_texture : ball_shot_texture,
        20,
        20,
        20,
        20,
        pos_x+(size_x/2)-10,
        pos_y+(size_y/2)-10,
        fire_step_x,
        fire_step_y,
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
    if (fire_step_x == 1 && fire_step_y == 1) {
        fire_step_y *= -1;
        move_shot_a = &move_shot_y_every;
        move_shot_b = &move_shot_x_every;
    } else if (fire_step_x == 1 && fire_step_y == -1) {
        fire_step_x *= -1;
        move_shot_a = &move_shot_x_every;
        move_shot_b = &move_shot_y_every;
    } else if (fire_step_x == -1 && fire_step_y == -1) {
        fire_step_y *= -1;
        move_shot_a = &move_shot_y_every;
        move_shot_b = &move_shot_x_every;
    } else if (fire_step_x == -1 && fire_step_y == 1) {
        fire_step_x *= -1;
        move_shot_a = &move_shot_x_every;
        move_shot_b = &move_shot_y_every;
    }

    *move_shot_a = SHOT_PHYS_DELAY_DIFF_START;
    *move_shot_b = SHOT_PHYS_DELAY_DEF;
}
