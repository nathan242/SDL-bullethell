#include "player_projectile.h"

extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;

void player_projectile::init()
{
    type_id = ID_PLAYER_SHOT;
    pos_x = 0;
    pos_y = 0;
    size_x = 2;
    size_y = 10;
    phys_size_x = 2;
    phys_size_y = 10;
    step_x = 0;
    step_y = -1;
    move_x_every = 0;
    move_y_every = 0;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = -1;
    collided = NULL;
    phys_active = false;
    draw_active = false;
    sprite = NULL;
    texture = NULL;
}

bool player_projectile::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    phys_active = false;
    draw_active = false;

    if (obj2 != NULL) {
        if (obj2->type_id > ID_PLAYER_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;
        }
    }

    return true;
}
