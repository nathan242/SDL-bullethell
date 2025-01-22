#include "enemy_adv.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000

extern int ID_ENEMY_SHIP;
extern int SHOT_PHYS_DELAY;

void enemy_adv::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 41;
    size_y = 41;
    phys_size_x = 41;
    phys_size_y = 41;
    area_y_offset = 41;
    move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    bounce = 1;
    last_shot = {0, 0};
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_adv_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_adv_hit_tex");

    last_hit = {0, 0};

    default_health = 12;
    current_health = 12;

    shot_delay = 400000000;

    drop_powerup = NULL;
    ungroup = false;

    ship_obj = (engine_obj*)i_eng->get_resource("ship_obj");
    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    init_projectile();

    initialized = true;
}

void enemy_adv::pre_phys_event()
{
    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    enemy::pre_phys_event();
}

void enemy_adv::fire()
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
}
