#include "enemy_cargo.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 14000000

extern int ID_ENEMY_SHIP;
extern int HIT_FLASH_DELAY;

void enemy_cargo::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 30;
    size_y = 50;
    phys_size_x = 30;
    phys_size_y = 50;
    area_y_offset = 50;
    move_x = add_timer(0);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_cargo_tex");
    default_shot_texture = NULL;

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_cargo_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 8;
    current_health = 8;

    shot_timer = add_timer(0);

    drop_powerup = NULL;
    ungroup = false;
    ungroup_at_y = 0;

    initialized = true;
}
