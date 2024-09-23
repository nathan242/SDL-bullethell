#include "ship.h"
#include <SDL2/SDL_image.h>

#define SHIP_MOVE_PHYS_DELAY 3000000

extern int ID_PLAYER_SHIP;
extern int SHOT_PHYS_DELAY;

ship::ship(engine *eng, projectile_manager *projectile_mngr)
{
    initialized = false;
    i_eng = eng;
    p_mngr = projectile_mngr;
}

void ship::init()
{
    type_id = ID_PLAYER_SHIP;
    pos_x = 50;
    pos_y = 550;
    size_x = 21;
    size_y = 21;
    phys_size_x = 21;
    phys_size_y = 21;
    step_x = 0;
    step_y = 0;
    move_x_every = SHIP_MOVE_PHYS_DELAY;
    move_y_every = SHIP_MOVE_PHYS_DELAY;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = 0;
    collided = NULL;
    phys_active = true;
    draw_active = true;
    sprite = IMG_Load("ship.png");
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    SDL_Surface *shot_sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
    SDL_FillRect(shot_sprite, NULL, SDL_MapRGB(shot_sprite->format, 0, 255, 0));
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);

    initialized = true;
}

void ship::fire()
{
    p_mngr->fire(
        default_shot_texture,
        2,
        10,
        2,
        10,
        pos_x+(size_x/2),
        pos_y-size_y,
        0,
        -1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY
    );
}

ship::~ship()
{
    if (initialized) {
        SDL_DestroyTexture(default_shot_texture);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(sprite);
    }
}