#include "shield.h"
#include <SDL2/SDL_image.h>

extern int ID_PLAYER_SHIELD;
extern int ID_ENEMY_SHOT;

shield::shield(engine *eng)
{
    initialized = false;
    i_eng = eng;
}

void shield::init()
{
    engine_obj::init();

    type_id = ID_PLAYER_SHIELD;
    size_x = 81;
    size_y = 81;
    phys_size_x = 59;
    phys_size_y = 59;
    phys_offset_x = 11;
    phys_offset_y = 11;
    area_y_offset = 100;
    area_x_offset = 100;
    bounce = 0;
    texture = (SDL_Texture*)i_eng->get_resource("shield_tex");
    activation_timer = add_timer(5000000000);
    target = NULL;

    initialized = true;
}

bool shield::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_ENEMY_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;
        }
    }

    return false;
}

void shield::pre_phys_event()
{
    if (draw_active) {
        if (!target->draw_active) {
            target = NULL;
            phys_active = false;
            draw_active = false;

            return;
        }

        pos_x = target->pos_x-(size_x/2)+(target->size_x/2)+target_offset_x;
        pos_y = target->pos_y-(size_y/2)+(target->size_y/2)+target_offset_y;

        if (activation_timer->tick(i_eng->timer_now)) {
            target = NULL;
            phys_active = false;
            draw_active = false;
        }
    }
}

void shield::activate(engine_obj *target_obj, int offset_x, int offset_y)
{
    if (!draw_active) {
        target = target_obj;
        target_offset_x = offset_x;
        target_offset_y = offset_y;
        phys_active = true;
        draw_active = true;

        activation_timer->last = {0, 0};
        activation_timer->tick(i_eng->timer_now);
    }
}

shield::~shield()
{
    if (initialized) {
        
    }
}
