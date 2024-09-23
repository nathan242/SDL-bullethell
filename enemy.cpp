#include "enemy.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 5000000

extern int ID_ENEMY_SHIP;
extern int ID_PLAYER_SHIP;
extern int ID_PLAYER_SHOT;
extern int ID_ENEMY_SHOT;
extern int SHOT_PHYS_DELAY;

extern bool reset_enemy;
extern bool game_over;

enemy::enemy(engine *eng, projectile_manager *projectile_mngr)
{
    initialized = false;
    i_eng = eng;
    p_mngr = projectile_mngr;
}

void enemy::init()
{
    type_id = ID_ENEMY_SHIP;
    pos_x = 150;
    pos_y = 150;
    size_x = 20;
    size_y = 20;
    phys_size_x = 20;
    phys_size_y = 20;
    step_x = 1;
    step_y = 1;
    move_x_every = ENEMY_SHIP_MOVE_PHYS_DELAY;
    move_y_every = ENEMY_SHIP_MOVE_PHYS_DELAY*2;
    move_x_last = {0, 0};
    move_y_last = {0, 0};
    bounce = 1;
    collided = NULL;
    phys_active = true;
    draw_active = true;
    sprite = SDL_CreateRGBSurface(0, 20, 20, 32, 0, 0, 0, 0);
    SDL_FillRect(sprite, NULL, SDL_MapRGB(sprite->format, 255, 0, 0));
    texture = SDL_CreateTextureFromSurface(i_eng->renderer, sprite);

    init_projectile();

    initialized = true;
}

void enemy::init_projectile()
{
    SDL_Surface *shot_sprite = SDL_CreateRGBSurface(0, 2, 10, 32, 0, 0, 0, 0);
    SDL_FillRect(shot_sprite, NULL, SDL_MapRGB(shot_sprite->format, 180, 0, 0));
    default_shot_texture = SDL_CreateTextureFromSurface(i_eng->renderer, shot_sprite);
    SDL_FreeSurface(shot_sprite);
}

bool enemy::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    if (obj2 != NULL) {
        if (obj2->type_id == ID_PLAYER_SHOT) {
            reset_enemy = true;
        } else if (obj2->type_id == ID_PLAYER_SHIP) {
            game_over = true;
        } else if (obj2->type_id == ID_ENEMY_SHOT) {
            return false;
        }
    } else if (collide_axis == 2 && pos_y > 0) {
        draw_active = false;
        phys_active = false;
    }

    return true;
}

void enemy::fire()
{
    p_mngr->fire(
        default_shot_texture,
        2,
        10,
        2,
        10,
        pos_x+(size_x/2),
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY
    );
}

enemy::~enemy()
{
    if (initialized) {
        SDL_DestroyTexture(default_shot_texture);
        SDL_DestroyTexture(texture);
        SDL_FreeSurface(sprite);
    }
}