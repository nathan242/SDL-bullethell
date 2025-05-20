#include "base_enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000

extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;
extern int ID_PLAYER_SHIELD;

extern bool game_over;

base_enemy::base_enemy(engine *eng, projectile_manager *projectile_mngr)
{
    initialized = false;
    i_eng = eng;
    p_mngr = projectile_mngr;
}

void base_enemy::init()
{
    engine_obj::init();

    drop_powerup = NULL;
    ungroup = false;
    ungroup_at_y = 0;

    ship_obj = (ship*)i_eng->get_resource("ship_obj");

    init_projectile();
}

void base_enemy::init_projectile()
{
    
}

bool base_enemy::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;

            damage(1);

            if (draw_active == false && drop_powerup != NULL && ship_obj->active_weapon < (drop_powerup->type_id - 99)) {
                drop_powerup->pos_x = pos_x;
                drop_powerup->pos_y = pos_y;
                drop_powerup->phys_active = true;
                drop_powerup->draw_active = true;
            }

            return false;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            obj2->phys_active = false;
            obj2->draw_active = false;
            game_over = true;
        } else if (obj2->type_id == ID_ENEMY_SHOT) {
            return false;
        } else if (obj2->type_id >= 100 && obj2->type_id <= 199) {
            return false;
        } else if (obj2->type_id == ID_PLAYER_SHIELD) {
            return false;
        }
    } else if (collide_axis == 2 && pos_y > 0) {
        draw_active = false;
        phys_active = false;
    }

    return true;
}

void base_enemy::pre_phys_event()
{
    if (ungroup_at_y > 0) {
        if (pos_y >= ungroup_at_y) {
            ungroup = true;
        }
    }

    if (pos_y > (size_y * -1) && shot_timer->tick(i_eng->timer_now)) {
        fire();
    }

    if (last_hit_timer->tick(i_eng->timer_now)) {
        texture = default_texture;
    }
}

void base_enemy::damage(int damage_amount)
{
    current_health -= damage_amount;
    if (current_health <= 0) {
        current_health = default_health;
        phys_active = false;
        draw_active = false;
    }

    texture = hit_texture;
    last_hit_timer->last = i_eng->timer_now;
}

void base_enemy::fire()
{
    
}

base_enemy::~base_enemy()
{
    if (initialized) {

    }
}