#include "player_projectile.h"

extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHIP;

void player_projectile::init()
{
    engine_obj::init();

    type_id = ID_PLAYER_SHOT;
    size_x = 2;
    size_y = 10;
    phys_size_x = 2;
    phys_size_y = 10;
    area_x_offset = 2;
    area_y_offset = 10;
    step_y = -1;
    bounce = -1;
}

bool player_projectile::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    phys_active = false;
    draw_active = false;

    if (obj2 != NULL) {
        if (obj2->type_id == ID_ENEMY_SHIP || obj2->type_id > ID_PLAYER_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;
        }
    }

    return true;
}
