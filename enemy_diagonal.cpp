#include "enemy_diagonal.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;
extern int HIT_FLASH_DELAY;

void enemy_diagonal::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 40;
    size_y = 40;
    phys_size_x = 40;
    phys_size_y = 40;
    area_y_offset = 40;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_diagonal_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 4;
    current_health = 4;

    shot_timer = add_timer(800000000);

    drop_powerup = NULL;
    ungroup = false;
    ungroup_at_y = 0;

    init_projectile();

    initialized = true;
}

void enemy_diagonal::init_projectile()
{
    default_shot_texture = NULL;
    default_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
}

void enemy_diagonal::fire()
{
    for (int x = -1; x < 2; x += 2) {
        for (int y = -1; y < 2; y += 2) {
            p_mngr->fire(
                default_shot_animation,
                20,
                20,
                20,
                20,
                x == 1 ? pos_x+size_x : pos_x-20,
                y == 1 ? pos_y+size_y : pos_y-20,
                x,
                y,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY,
                false
            );
        }
    }
}
