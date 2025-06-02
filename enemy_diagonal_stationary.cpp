#include "enemy_diagonal_stationary.h"
#include <SDL2/SDL_image.h>
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define ENEMY_SHOT_PHYS_DELAY 5000000

void enemy_diagonal_stationary::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 40;
    size_y = 40;
    phys_size_x = 40;
    phys_size_y = 40;
    area_y_offset = 40;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_6_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_6_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 8;
    current_health = 8;

    shot_timer = add_timer(800000000);

    initialized = true;
}

void enemy_diagonal_stationary::init_projectile()
{
    default_shot_texture = NULL;
    default_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
}

void enemy_diagonal_stationary::pre_phys_event()
{
    if (pos_y >= 200) {
        step_y = 0;
    }

    base_enemy::pre_phys_event();
}

void enemy_diagonal_stationary::fire()
{
    int fire_pos_x;
    int fire_pos_y;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            fire_pos_x = x == 1 ? pos_x+size_x : pos_x-20;
            fire_pos_y = y == 1 ? pos_y+size_y : pos_y-20;

            if (x == 0 && y == 0) {
                continue;
            } else if (x == 0) {
                fire_pos_x = pos_x+(size_x/2)-10;
            } else if (y == 0) {
                fire_pos_y = pos_y+(size_y/2)-10;
            }

            p_mngr->fire(
                default_shot_animation,
                20,
                20,
                20,
                20,
                fire_pos_x,
                fire_pos_y,
                x,
                y,
                ENEMY_SHOT_PHYS_DELAY,
                ENEMY_SHOT_PHYS_DELAY,
                false
            );
        }
    }
}
