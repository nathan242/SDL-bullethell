#include "enemy_projectile.h"
#include "constants.h"

extern bool game_over;

enemy_projectile::enemy_projectile(explosion_manager *explosion_mngr)
{
    e_mngr = explosion_mngr;
}

void enemy_projectile::init()
{
    base_projectile::init();

    move_x = add_timer(0);
    move_y = add_timer(0);

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
            obj2->phys_active = false;
            obj2->draw_active = false;
            game_over = true;

            e_mngr->explode(obj2->size_x, obj2->size_y, obj2->pos_x, obj2->pos_y);
        }
    } else {
        phys_active = false;
        draw_active = false;
    }

    return false;
}
