#include "enemy_projectile.h"

extern int ID_PLAYER_SHIP;
extern int ID_ENEMY_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;

extern bool game_over;

enemy_projectile::enemy_projectile()
{
    type_id = ID_ENEMY_SHOT;
    pos_x = 0;
    pos_y = 0;
    size_x = 2;
    size_y = 10;
    phys_size_x = 2;
    phys_size_y = 10;
    step_x = 0;
    step_y = 1;
    move_x_every = 0;
    move_y_every = 0;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = -1;
    collided = NULL;
    callback = enemy_projectile_callback;
    phys_active = false;
    draw_active = false;
    sprite = NULL;
    texture = NULL;
}

bool enemy_projectile_callback(engine_obj *obj, engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT) {
            obj->phys_active = false;
            obj->draw_active = false;
            obj2->phys_active = false;
            obj2->draw_active = false;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        }
    } else {
        obj->phys_active = false;
        obj->draw_active = false;
    }

    return false;
}

enemy_projectile::~enemy_projectile()
{
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(sprite);
}