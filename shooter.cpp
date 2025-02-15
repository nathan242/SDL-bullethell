#include "engine.h"
#include "background.h"
#include "menu_title.h"
#include "press_key.h"
#include "ship.h"
#include "enemy.h"
#include "enemy_adv.h"
#include "enemy_diagonal.h"
#include "enemy_diagonal_stationary.h"
#include "enemy_diagonal_stationary_fwdsprd.h"
#include "enemy_cargo.h"
#include "enemy_boss_a.h"
#include "powerup_double_shot.h"
#include "powerup_quad_spread_shot.h"
#include "player_projectile.h"
#include "enemy_projectile.h"
#include "projectile_manager.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <time.h>
#include <unistd.h>
#include <unordered_map>
#include <string>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define RES_X 800
#define RES_Y 600
#define BPP 32

#define NUM_SHOTS 100
#define NUM_SHOTS_ENEMY 100
#define ENEMY_SET_COUNT 8
#define MAX_ENEMY_SLOTS 10
#define AUTO_FIRE_DELAY 100000000

#define MENU_QUIT 0
#define MENU_START 1

// Emscripten loop handling
bool em_game_started = false;

int ID_PLAYER_SHIP = 1;
int ID_ENEMY_SHIP = 2;
int ID_POWERUP_DOUBLE_SHOT = 100;
int ID_POWERUP_QUAD_SPREAD_SHOT = 101;
int ID_PLAYER_SHOT = 1000;
int ID_ENEMY_SHOT = 1001;
int ID_BACKGROUND = 2000;

int SHOT_PHYS_DELAY = 2500000;

// Inputs
bool quit = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
bool fire = false;

bool game_over = false;
bool fired = false;
timespec now;
timespec player_last_shot;
uint64_t timediff;
int active_enemy_set = -1;
SDL_Event input;
char *base_path;
engine *eng;

// Menu objects
menu_title *title_obj;
press_key *press_key_obj;

// Game objects
projectile_manager *player_shot_mngr;
projectile_manager *enemy_shot_mngr;
background *background_a_obj;
background *background_b_obj;
ship *ship_obj;
player_projectile *shots[NUM_SHOTS];
enemy_projectile *enemy_shots[NUM_SHOTS_ENEMY];
powerup_double_shot *powerup_double_shot_obj;
powerup_quad_spread_shot *powerup_quad_spread_shot_obj;
engine_obj_list *enemy_slots[MAX_ENEMY_SLOTS];

std::unordered_map<std::string, std::string> texture_map = {
    {"background_tex", "background.png"},
    {"menu_title_tex", "title.png"},
    {"press_key_tex", "press_a_key.png"},
    {"ship_tex", "ship.png"},
    {"projectile_player_default_tex", "projectile_player_default.png"},
    {"enemy_ship_default_tex", "enemy_ship_default.png"},
    {"enemy_ship_default_hit_tex", "enemy_ship_default_hit.png"},
    {"enemy_ship_adv_tex", "enemy_ship_adv.png"},
    {"enemy_ship_adv_hit_tex", "enemy_ship_adv_hit.png"},
    {"projectile_default_tex", "projectile_default.png"},
    {"enemy_ship_diagonal_tex", "enemy_ship_diagonal.png"},
    {"enemy_ship_diagonal_hit_tex", "enemy_ship_diagonal_hit.png"},
    {"projectile_ball_tex", "projectile_ball.png"},
    {"enemy_ship_diagonal_6_tex", "enemy_ship_diagonal_6.png"},
    {"enemy_ship_diagonal_6_hit_tex", "enemy_ship_diagonal_6_hit.png"},
    {"projectile_ball_invincible_tex", "projectile_ball_invincible.png"},
    {"enemy_cargo_tex", "enemy_cargo.png"},
    {"enemy_cargo_hit_tex", "enemy_cargo_hit.png"},
    {"powerup_double_shot_tex", "powerup_double_shot.png"},
    {"powerup_quad_spread_shot_tex", "powerup_quad_spread_shot.png"}
};

void init_resources()
{
    SDL_Surface *temp_surface;

    for (const auto& [name, texture] : texture_map) {
        temp_surface = IMG_Load(texture.c_str());
        eng->add_resource(name.c_str(), SDL_CreateTextureFromSurface(eng->renderer, temp_surface));
        SDL_FreeSurface(temp_surface);
    }
}

void free_resources()
{
    for (const auto& [name, texture] : texture_map) {
        SDL_DestroyTexture((SDL_Texture*)eng->get_resource(name.c_str()));
    }
}

int get_enemy_slot()
{
    int slot = 0;

    for (int i = 0; i < MAX_ENEMY_SLOTS; i++) {
        if (!enemy_slots[i]->obj->draw_active) {
            delete enemy_slots[i]->obj;
            slot = i;

            break;
        }
    }

    return slot;
}

void activate_enemy_set(int set_id)
{
    int slot;

    switch (set_id)
    {
        case 0:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 250;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = -1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 550;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-8;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 1:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 40;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 80;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 120;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-5;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 160;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 240;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-9;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 320;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-11;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 2:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 300;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-2;
            enemy_slots[slot]->obj->step_x = -1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 100;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_cargo(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            ((enemy*)enemy_slots[slot]->obj)->drop_powerup = powerup_double_shot_obj;
            ((enemy*)enemy_slots[slot]->obj)->ungroup = true;
            enemy_slots[slot]->obj->pos_x = 650;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 3:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 160;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 320;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 480;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 640;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 300;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = -1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 100;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-5;
            enemy_slots[slot]->obj->step_x = -1;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 4:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 550;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 350;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-8;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 5:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 650;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-2;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 6:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 550;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 7:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 650;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-5;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-6;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 8:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_boss_a(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 350;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            ((enemy*)enemy_slots[slot]->obj)->drop_powerup = powerup_quad_spread_shot_obj;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;
    }
}

void get_input()
{
    while (SDL_PollEvent(&input)) {
        switch (input.type) {
            case SDL_KEYDOWN:
                switch (input.key.keysym.sym) {
                    case SDLK_LEFT:
                        left = true;
                        break;
                    case SDLK_RIGHT:
                        right = true;
                        break;
                    case SDLK_UP:
                        up = true;
                        break;
                    case SDLK_DOWN:
                        down = true;
                        break;
                    case SDLK_SPACE:
                        fire = true;
                        break;
                    case SDLK_q:
                        quit = true;
                        break;
                }

                break;
            case SDL_KEYUP:
                switch (input.key.keysym.sym) {
                    case SDLK_LEFT:
                        left = false;
                        break;
                    case SDLK_RIGHT:
                        right = false;
                        break;
                    case SDLK_UP:
                        up = false;
                        break;
                    case SDLK_DOWN:
                        down = false;
                        break;
                    case SDLK_SPACE:
                        fire = false;
                        break;
                }
        }
    }
}

void init()
{
    player_last_shot = {0, 0};

    base_path = SDL_GetBasePath();

    chdir(base_path);
    free(base_path);

    eng = new engine("SDL SHOOTER", RES_X, RES_Y, BPP);

    init_resources();

    // Game objects
    player_shot_mngr = new projectile_manager();
    enemy_shot_mngr = new projectile_manager();
    background_a_obj = new background(eng);
    background_b_obj = new background(eng);
    ship_obj = new ship(eng, player_shot_mngr);

    eng->add_object(background_a_obj);
    background_a_obj->init();
    eng->add_object(background_b_obj);
    background_b_obj->init();
    background_b_obj->pos_y = background_b_obj->size_y*-1;

    for (int i = 0; i < MAX_ENEMY_SLOTS; i++) {
        enemy_slots[i] = eng->add_object(new engine_obj());
    }

    powerup_double_shot_obj = new powerup_double_shot(eng);
    powerup_double_shot_obj->init();
    eng->add_object(powerup_double_shot_obj);

    powerup_quad_spread_shot_obj = new powerup_quad_spread_shot(eng);
    powerup_quad_spread_shot_obj->init();
    eng->add_object(powerup_quad_spread_shot_obj);

    eng->add_object(ship_obj);
    ship_obj->init();

    eng->add_resource("ship_obj", ship_obj);

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        shots[shot_count] = new player_projectile();
        shots[shot_count]->init();
        eng->add_object(shots[shot_count]);
        player_shot_mngr->add_object(shots[shot_count]);
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        // Shots
        enemy_shots[enemy_shot_count] = new enemy_projectile();
        enemy_shots[enemy_shot_count]->init();
        eng->add_object(enemy_shots[enemy_shot_count]);
        enemy_shot_mngr->add_object(enemy_shots[enemy_shot_count]);
    }

    // Menu objects
    title_obj = new menu_title(eng);
    eng->add_object(title_obj);
    title_obj->init();
    press_key_obj = new press_key(eng);
    eng->add_object(press_key_obj);
    press_key_obj->init();
}

void menu_loop()
{
    ship_obj->phys_active = false;
    ship_obj->draw_active = false;

    title_obj->draw_active = true;
    press_key_obj->draw_active = true;

    get_input();

    if (left | right | up | down | fire) {
        title_obj->draw_active = false;
        press_key_obj->draw_active = false;

        em_game_started = true;
        ship_obj->reset();
    } else if (quit) {
        em_game_started = true;
        ship_obj->reset();
    }

    eng->step();
}

void game_loop()
{
    bool init_enemy_set = false;
    // activate_enemy_set(active_enemy_set);

    // ship_obj->reset();

    if (quit == false) {
        get_input();

        if (!game_over) {
            ship_obj->step_x = 0;
            ship_obj->step_y = 0;
            if (left) { ship_obj->step_x = -1; }
            if (right) { ship_obj->step_x = 1; }
            if (up) { ship_obj->step_y = -1; }
            if (down) { ship_obj->step_y = 1; }

            if (fire) {
                clock_gettime(CLOCK_MONOTONIC, &now);
                timediff = ((now.tv_sec - player_last_shot.tv_sec) * 1000000000) + (now.tv_nsec - player_last_shot.tv_nsec);

                if (!fired || timediff > AUTO_FIRE_DELAY) {
                    fired = true;
                    ship_obj->fire();

                    player_last_shot = now;
                }
            } else {
                fired = false;
            }

            init_enemy_set = true;

            for (int i = 0; i < MAX_ENEMY_SLOTS; i++) {
                if (enemy_slots[i]->obj->draw_active && !(enemy_slots[i]->obj->type_id == ID_ENEMY_SHIP && ((enemy*)enemy_slots[i]->obj)->ungroup)) {
                    init_enemy_set = false;
                }
            }

            // Loop round enemy sets for now
            if (init_enemy_set && ++active_enemy_set > ENEMY_SET_COUNT) {
                active_enemy_set = 0;
            }

            if (init_enemy_set) {
                activate_enemy_set(active_enemy_set);
            }

            // Redraw screen
            eng->step();
        }
    } else {
        emscripten_cancel_main_loop();
    }
}

void deinit()
{
    SDL_Quit();

    delete background_a_obj;
    delete background_b_obj;
    delete title_obj;
    delete ship_obj;
    delete player_shot_mngr;
    delete enemy_shot_mngr;
    delete powerup_double_shot_obj;
    delete powerup_quad_spread_shot_obj;

    for (int shot_count = 0; shot_count < NUM_SHOTS; shot_count++) {
        delete shots[shot_count];
    }

    for (int enemy_shot_count = 0; enemy_shot_count < NUM_SHOTS_ENEMY; enemy_shot_count++) {
        delete enemy_shots[enemy_shot_count];
    }

    for (int i = 0; i < MAX_ENEMY_SLOTS; i++) {
        delete enemy_slots[i]->obj;
    }

    free_resources();

    delete eng;
}

void em_loop()
{
    if (em_game_started) {
        game_loop();
    } else {
        menu_loop();
    }
}

int main (int argc, char *argv[])
{
    init();

    // emscripten_set_main_loop_timing(EM_TIMING_RAF, 1);
    emscripten_set_main_loop(em_loop, 0, 1);
    // emscripten_set_main_loop(game_loop, 0, 1);

    // switch(menu_loop()) {
    //     case MENU_START:
    //         game_loop();
    //         break;
    // }

    deinit();

    return 0;
}

