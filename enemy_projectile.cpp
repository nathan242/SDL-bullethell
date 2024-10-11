#include "enemy_projectile.h"

extern int ID_PLAYER_SHIP;
extern int ID_ENEMY_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;

extern bool game_over;

void enemy_projectile::init()
{
    base_projectile::init();

    type_id = ID_ENEMY_SHOT;
    bounce = -1;
}

bool enemy_projectile::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT && !invincible) {
            phys_active = false;
            draw_active = false;
            obj2->phys_active = false;
            obj2->draw_active = false;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        }
    } else {
        phys_active = false;
        draw_active = false;
    }

    return false;
}
