#include "enemy.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000

extern int ID_ENEMY_SHIP;
extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;
extern int SHOT_PHYS_DELAY;
extern int HIT_FLASH_DELAY;

extern bool game_over;

enemy::enemy(engine *eng, projectile_manager *projectile_mngr)
{
    initialized = false;
    i_eng = eng;
    p_mngr = projectile_mngr;
}

void enemy::init()
{
    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 20;
    size_y = 20;
    phys_size_x = 20;
    phys_size_y = 20;
    area_y_offset = 20;
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

    drop_powerup = NULL;
    ungroup = false;
    ungroup_at_y = 0;

    init_projectile();

    initialized = true;
}

void enemy::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
}

bool enemy::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT) {
            obj2->phys_active = false;
            obj2->draw_active = false;

            damage(1);

            if (draw_active == false && drop_powerup != NULL) {
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
        }
    } else if (collide_axis == 2 && pos_y > 0) {
        draw_active = false;
        phys_active = false;
    }

    return true;
}

void enemy::pre_phys_event()
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

void enemy::damage(int damage_amount)
{
    current_health -= damage_amount;
    if (current_health <= 0) {
        current_health = default_health;
        phys_active = false;
        draw_active = false;
    }

    texture = hit_texture;
    clock_gettime(CLOCK_MONOTONIC, &last_hit_timer->last);
}

void enemy::fire()
{
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2),
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