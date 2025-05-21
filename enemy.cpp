#include "enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000

extern int ID_ENEMY_SHIP;
extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;
extern int ID_PLAYER_SHIELD;
extern int SHOT_PHYS_DELAY;
extern int HIT_FLASH_DELAY;

extern bool game_over;

void enemy::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 21;
    size_y = 31;
    phys_size_x = 21;
    phys_size_y = 31;
    area_y_offset = 21;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_default_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_default_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 2;
    current_health = 2;

    // shot_delay = 800000000;
    shot_timer = add_timer(800000000);

    initialized = true;
}

void enemy::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
    default_shot_animation = NULL;
}

void enemy::fire()
{
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-2,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );
}

enemy::~enemy()
{
    if (initialized) {

    }
}