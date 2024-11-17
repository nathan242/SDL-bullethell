#include "enemy_cargo.h"
#include <SDL2/SDL_image.h>

#define ENEMY_SHIP_MOVE_PHYS_DELAY 14000000

extern int ID_ENEMY_SHIP;

void enemy_cargo::init()
{
    SDL_Surface *hit_sprite;

    engine_obj::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 30;
    size_y = 50;
    phys_size_x = 30;
    phys_size_y = 50;
    area_y_offset = 50;
    move_x_every = 0;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    bounce = 1;
    sprite = IMG_Load("enemy_cargo.png");
    last_shot = {0, 0};
    default_texture = texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);
    default_shot_texture = NULL;

    hit_sprite = IMG_Load("enemy_cargo_hit.png");
    hit_texture = SDL_CreateTextureFromSurface(i_eng->renderer, hit_sprite);
    SDL_FreeSurface(hit_sprite);

    last_hit = {0, 0};

    default_health = 8;
    current_health = 8;

    shot_delay = 0;

    initialized = true;
}

void enemy_cargo::damage(int damage_amount)
{
    enemy::damage(damage_amount);

    if (draw_active == false) {
        drop_powerup->pos_x = pos_x;
        drop_powerup->pos_y = pos_y;
        drop_powerup->phys_active = true;
        drop_powerup->draw_active = true;
    }
}
