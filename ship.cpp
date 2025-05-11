#include "ship.h"
#include <SDL2/SDL_image.h>

#define SHIP_MOVE_PHYS_DELAY 3000000

extern int ID_PLAYER_SHIP;
extern int SHOT_PHYS_DELAY;

ship::ship(engine *eng, projectile_manager *projectile_mngr)
{
    initialized = false;
    i_eng = eng;
    p_mngr = projectile_mngr;
}

void ship::init()
{
    engine_obj::init();

    type_id = ID_PLAYER_SHIP;
    size_x = 21;
    size_y = 21;
    phys_size_x = 21;
    phys_size_y = 21;
    move_x = add_timer(SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(SHIP_MOVE_PHYS_DELAY);
    texture = (SDL_Texture*)i_eng->get_resource("ship_tex");

    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_player_default_tex");

    reset();

    initialized = true;
}

void ship::reset()
{
    pos_x = 390;
    pos_y = 550;

    active_weapon = 0;

    phys_active = true;
    draw_active = true;
}

bool ship::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL && obj2->type_id >= 100 && obj2->type_id <= 199) {
        obj2->phys_active = false;
        obj2->draw_active = false;

        active_weapon = obj2->type_id - 99;

        return false;
    }

    return true;
}

void ship::fire()
{
    switch (active_weapon) {
        case 1:
            p_mngr->fire(
                default_shot_texture,
                5,
                10,
                5,
                10,
                pos_x+(size_x/2)-10,
                pos_y-size_y,
                0,
                -1,
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
                pos_y-size_y,
                0,
                -1,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY,
                false
            );

            break;

        case 2:
            // Forward left
            p_mngr->fire(
                default_shot_texture,
                5,
                10,
                5,
                10,
                pos_x+(size_x/2)-10,
                pos_y-size_y,
                0,
                -1,
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
                pos_x+(size_x/2)+6,
                pos_y-size_y,
                0,
                -1,
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
                pos_x+(size_x/2)-10,
                pos_y-size_y,
                -1,
                -1,
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
                pos_x+(size_x/2)+6,
                pos_y-size_y,
                1,
                -1,
                SHOT_PHYS_DELAY*8,
                SHOT_PHYS_DELAY,
                false
            );

            break;

        default:
            p_mngr->fire(
                default_shot_texture,
                5,
                10,
                5,
                10,
                pos_x+(size_x/2)-2,
                pos_y-size_y,
                0,
                -1,
                SHOT_PHYS_DELAY,
                SHOT_PHYS_DELAY,
                false
            );
    }
}

ship::~ship()
{
    if (initialized) {

    }
}