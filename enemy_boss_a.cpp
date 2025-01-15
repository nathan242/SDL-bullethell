#include "enemy_boss_a.h"
#include <SDL2/SDL_image.h>
#include <iostream>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
//#define SHOT_PHYS_DELAY 5000000
#define SHOT_PHYS_DELAY 10000000
#define SHOT_PHYS_DELAY_DIFF 8000000
#define SHOT_PHYS_DELAY_DIFF_START 100000000

extern int ID_ENEMY_SHIP;

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
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_hit_tex");

    last_hit = {0, 0};

    default_health = 1000;
    current_health = 1000;

    shot_delay = 200000000;

    drop_powerup = NULL;
    ungroup = false;

    init_projectile();

    initialized = true;
}

void enemy_boss_a::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_ball_tex");
    move_shot_a = &move_shot_x_every;
    move_shot_b = &move_shot_y_every;
    *move_shot_a = SHOT_PHYS_DELAY_DIFF_START;
    *move_shot_b = SHOT_PHYS_DELAY;
    diff_shot_direction = 0;
    fire_step_x = 1;
    fire_step_y = 1;
}

void enemy_boss_a::pre_phys_event()
{
    if (pos_y >= 200) {
        step_y = 0;
    }

    enemy::pre_phys_event();
}

void enemy_boss_a::fire()
{
    // int fire_pos_x;
    // int fire_pos_y;

    // for (int x = -1; x < 2; x++) {
    //     for (int y = -1; y < 2; y++) {
    //         fire_pos_x = x == 1 ? pos_x+size_x : pos_x-60;
    //         fire_pos_y = y == 1 ? pos_y+size_y : pos_y-60;

    //         if (x == 0 && y == 0) {
    //             continue;
    //         } else if (x == 0) {
    //             fire_pos_x = pos_x+(size_x/2)-10;
    //         } else if (y == 0) {
    //             fire_pos_y = pos_y+(size_y/2)-10;
    //         }

    //         p_mngr->fire(
    //             default_shot_texture,
    //             20,
    //             20,
    //             20,
    //             20,
    //             fire_pos_x,
    //             fire_pos_y,
    //             x,
    //             y,
    //             SHOT_PHYS_DELAY,
    //             SHOT_PHYS_DELAY,
    //             false
    //         );
    //     }
    // }

    // if (diff_shot_direction == 0) {
    //     move_shot_x_every -= SHOT_PHYS_DELAY_DIFF;
    //     move_shot_y_every += SHOT_PHYS_DELAY_DIFF;
    // } else {
    //     move_shot_x_every += SHOT_PHYS_DELAY_DIFF;
    //     move_shot_y_every -= SHOT_PHYS_DELAY_DIFF;
    // }

    // if (move_shot_x_every <= SHOT_PHYS_DELAY) {
    //     diff_shot_direction = 1;
    //     update_fire_step();
    // } else if (move_shot_y_every <= SHOT_PHYS_DELAY) {
    //     diff_shot_direction = 0;
    //     update_fire_step();
    // }

    if (diff_shot_direction == 0) {
        *move_shot_a -= SHOT_PHYS_DELAY_DIFF;
    } else {
        *move_shot_b += SHOT_PHYS_DELAY_DIFF;
    }

    if (diff_shot_direction == 0 && *move_shot_a <= SHOT_PHYS_DELAY) {
        *move_shot_a = SHOT_PHYS_DELAY;
        diff_shot_direction = 1;
        // update_fire_step();
    } else if (diff_shot_direction == 1 && *move_shot_b >= SHOT_PHYS_DELAY_DIFF_START) {
        *move_shot_b = SHOT_PHYS_DELAY;
        *move_shot_a = SHOT_PHYS_DELAY_DIFF_START;
        diff_shot_direction = 0;
        update_fire_step();
    }

    std::cout << move_shot_x_every << " : " << move_shot_y_every << " T: " << (move_shot_x_every + move_shot_y_every) << std::endl;

    p_mngr->fire(
        default_shot_texture,
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
        false
    );
}

// void enemy_boss_a::update_fire_step()
// {
//     // std::cout << fire_step_x << " : " << fire_step_y << std::endl;

//     if (fire_step_x == 1 && fire_step_y == 1) {
//         fire_step_y *= -1;
//     } else if (fire_step_x == 1 && fire_step_y == -1) {
//         fire_step_x *= -1;
//     } else if (fire_step_x == -1 && fire_step_y == -1) {
//         fire_step_y *= -1;
//     } else if (fire_step_x == -1 && fire_step_y == 1) {
//         fire_step_x *= -1;
//     }
// }

void enemy_boss_a::update_fire_step()
{
    std::cout << fire_step_x << " : " << fire_step_y << std::endl;

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
    *move_shot_b = SHOT_PHYS_DELAY;
}
