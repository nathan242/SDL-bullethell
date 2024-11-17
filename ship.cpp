#include "ship.h"
#include <SDL2/SDL_image.h>

#define SHIP_MOVE_PHYS_DELAY 3000000

extern int ID_PLAYER_SHIP;
extern int SHOT_PHYS_DELAY;
extern int ID_POWERUP_DOUBLE_SHOT;

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
    pos_x = 50;
    pos_y = 550;
    size_x = 21;
    size_y = 21;
    phys_size_x = 21;
    phys_size_y = 21;
    move_x_every = SHIP_MOVE_PHYS_DELAY;
    move_y_every = SHIP_MOVE_PHYS_DELAY;
    phys_active = true;
    draw_active = true;
    sprite = IMG_Load("ship.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    SDL_Surface *shot_sprite = IMG_Load("projectile_player_default.png");
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);

    active_weapon = 0;

    initialized = true;
}

bool ship::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL && obj2->type_id == ID_POWERUP_DOUBLE_SHOT) {
        obj2->phys_active = false;
        obj2->draw_active = false;

        active_weapon = 1;

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
                pos_x+(size_x/2)-6,
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

        default:
            p_mngr->fire(
                default_shot_texture,
                5,
                10,
                5,
                10,
                pos_x+(size_x/2),
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
        SDL_DestroyTexture(default_shot_texture);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(sprite);
    }
}