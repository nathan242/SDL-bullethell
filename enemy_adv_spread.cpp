#include "enemy_adv_spread.h"
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 9000000

void enemy_adv_spread::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 42;
    size_y = 43;
    phys_size_x = 42;
    phys_size_y = 43;
    area_y_offset = 43;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*3);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_adv_spread_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_adv_spread_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 18;
    current_health = 18;

    shot_timer = add_timer(400000000);

    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    initialized = true;
}

void enemy_adv_spread::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
    default_shot_animation = NULL;
}

void enemy_adv_spread::pre_phys_event()
{
    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    base_enemy::pre_phys_event();
}

void enemy_adv_spread::fire()
{
    // Forward left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-12,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Forward right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+7,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Angle left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-12,
        pos_y+size_y+1,
        -1,
        1,
        SHOT_PHYS_DELAY*8,
        SHOT_PHYS_DELAY,
        false
    );

    // Angle right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+7,
        pos_y+size_y+1,
        1,
        1,
        SHOT_PHYS_DELAY*8,
        SHOT_PHYS_DELAY,
        false
    );
}
