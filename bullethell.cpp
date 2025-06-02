#include "constants.h"
#include "engine.h"
#include "background.h"
#include "menu_title.h"
#include "press_key.h"
#include "game_over.h"
#include "paused_img.h"
#include "game_ui.h"
#include "anim_projectile_ball.h"
#include "anim_projectile_ball_invincible.h"
#include "anim_powerup_double_shot.h"
#include "anim_powerup_quad_spread_shot.h"
#include "ship.h"
#include "shield.h"
#include "enemy.h"
#include "enemy_adv.h"
#include "enemy_diagonal.h"
#include "enemy_diagonal_stationary.h"
#include "enemy_diagonal_stationary_spiral.h"
#include "enemy_diagonal_stationary_allsprd.h"
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
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#define RES_X 800
#define RES_Y 600
#define BPP 32

#define NUM_SHOTS 100
#define NUM_SHOTS_ENEMY 500
#define ENEMY_SET_COUNT 10
#define MAX_ENEMY_SLOTS 10
#define AUTO_FIRE_DELAY 100000000

#define MENU_QUIT 0
#define MENU_START 1

#ifdef __EMSCRIPTEN__
// Emscripten loop handling
bool em_game_started = false;
#endif

// Inputs
bool quit = false;
bool left = false;
bool right = false;
bool up = false;
bool down = false;
bool fire = false;
bool shield_btn = false;
bool pause_btn = false;

bool game_over = false;
bool fired = false;
bool pause_pressed = false;
bool paused = false;
int active_enemy_set;
SDL_Event input;
char *base_path;
engine *eng;

// Menu objects
menu_title *title_obj;
press_key *press_key_obj;
class game_over *game_over_obj;
paused_img *paused_img_obj;
game_ui *game_ui_obj;

// Game objects
projectile_manager *player_shot_mngr;
projectile_manager *enemy_shot_mngr;
background *background_a_obj;
background *background_b_obj;
ship *ship_obj;
shield *shield_obj;
timer_obj *ship_fire_timer;
player_projectile *shots[NUM_SHOTS];
enemy_projectile *enemy_shots[NUM_SHOTS_ENEMY];
powerup_double_shot *powerup_double_shot_obj;
powerup_quad_spread_shot *powerup_quad_spread_shot_obj;
engine_obj_list *enemy_slots[MAX_ENEMY_SLOTS];

std::unordered_map<std::string, std::string> texture_map = {
    {"background_tex", "background.png"},
    {"menu_title_tex", "title.png"},
    {"press_key_tex", "press_a_key.png"},
    {"game_over_tex", "game_over.png"},
    {"paused_tex", "paused.png"},
    {"game_ui_bar_tex", "game_ui_bar.png"},
    {"green_bar_tex", "green_bar.png"},
    {"red_bar_tex", "red_bar.png"},
    {"ship_tex", "ship.png"},
    {"shield_tex", "shield.png"},
    {"projectile_player_default_tex", "projectile_player_default.png"},
    {"enemy_ship_default_tex", "enemy_ship_default.png"},
    {"enemy_ship_default_hit_tex", "enemy_ship_default_hit.png"},
    {"enemy_ship_adv_tex", "enemy_ship_adv.png"},
    {"enemy_ship_adv_hit_tex", "enemy_ship_adv_hit.png"},
    {"projectile_default_tex", "projectile_default.png"},
    {"enemy_ship_diagonal_tex", "enemy_ship_diagonal.png"},
    {"enemy_ship_diagonal_hit_tex", "enemy_ship_diagonal_hit.png"},
    {"enemy_ship_boss_a_tex", "enemy_ship_boss_a.png"},
    {"enemy_ship_boss_a_hit_tex", "enemy_ship_boss_a_hit.png"},
    {"projectile_ball_tex", "projectile_ball.png"},
    {"projectile_ball_frame_1_tex", "projectile_ball_1.png"},
    {"projectile_ball_frame_2_tex", "projectile_ball_2.png"},
    {"projectile_ball_frame_3_tex", "projectile_ball_3.png"},
    {"projectile_ball_frame_4_tex", "projectile_ball_4.png"},
    {"projectile_ball_frame_5_tex", "projectile_ball_5.png"},
    {"enemy_ship_diagonal_6_tex", "enemy_ship_diagonal_6.png"},
    {"enemy_ship_diagonal_6_hit_tex", "enemy_ship_diagonal_6_hit.png"},
    {"projectile_ball_invincible_tex", "projectile_ball_invincible.png"},
    {"projectile_ball_invincible_frame_1_tex", "projectile_ball_invincible_1.png"},
    {"projectile_ball_invincible_frame_2_tex", "projectile_ball_invincible_2.png"},
    {"projectile_ball_invincible_frame_3_tex", "projectile_ball_invincible_3.png"},
    {"projectile_ball_invincible_frame_4_tex", "projectile_ball_invincible_4.png"},
    {"projectile_ball_invincible_frame_5_tex", "projectile_ball_invincible_5.png"},
    {"enemy_cargo_tex", "enemy_cargo.png"},
    {"enemy_cargo_hit_tex", "enemy_cargo_hit.png"},
    {"powerup_single_shot_tex", "powerup_single_shot.png"},
    {"powerup_double_shot_tex", "powerup_double_shot.png"},
    {"powerup_double_shot_frame_1_tex", "powerup_double_shot_1.png"},
    {"powerup_double_shot_frame_2_tex", "powerup_double_shot_2.png"},
    {"powerup_quad_spread_shot_tex", "powerup_quad_spread_shot.png"},
    {"powerup_quad_spread_shot_frame_1_tex", "powerup_quad_spread_shot_1.png"},
    {"powerup_quad_spread_shot_frame_2_tex", "powerup_quad_spread_shot_2.png"}
};

void init_resources()
{
    SDL_Surface *temp_surface;

    for (const auto& [name, texture] : texture_map) {
        temp_surface = IMG_Load(texture.c_str());
        eng->add_resource(name.c_str(), SDL_CreateTextureFromSurface(eng->renderer, temp_surface));
        SDL_FreeSurface(temp_surface);
    }

    eng->add_resource("projectile_ball_anim", new anim_projectile_ball(new timer_obj(0), eng));
    eng->add_resource("projectile_ball_invincible_anim", new anim_projectile_ball_invincible(new timer_obj(0), eng));
    eng->add_resource("powerup_double_shot_anim", new anim_powerup_double_shot(new timer_obj(0), eng));
    eng->add_resource("powerup_quad_spread_shot_anim", new anim_powerup_quad_spread_shot(new timer_obj(0), eng));
}

void free_resources()
{
    for (const auto& [name, texture] : texture_map) {
        SDL_DestroyTexture((SDL_Texture*)eng->get_resource(name.c_str()));
    }

    delete (animation_obj*)eng->get_resource("projectile_ball_anim");
    delete (animation_obj*)eng->get_resource("projectile_ball_invincible_anim");
    delete (animation_obj*)eng->get_resource("powerup_double_shot_anim");
    delete (animation_obj*)eng->get_resource("powerup_quad_spread_shot_anim");
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
            ((enemy*)enemy_slots[slot]->obj)->ungroup_at_y = 400;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 320;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;
            ((enemy*)enemy_slots[slot]->obj)->ungroup_at_y = 400;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 480;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;
            ((enemy*)enemy_slots[slot]->obj)->ungroup_at_y = 400;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 640;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-14;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;
            ((enemy*)enemy_slots[slot]->obj)->ungroup_at_y = 400;

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
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_spiral(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 130;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 630;
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
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_spiral(eng, enemy_shot_mngr);
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
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_allsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 130;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_fwdsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 630;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-8;
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

        case 9:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 100;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 150;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-2;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-3;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 700;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-4;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_adv(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 650;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-5;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            break;

        case 10:
            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_allsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 130;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_allsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 380;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
            enemy_slots[slot]->obj->step_x = 0;
            enemy_slots[slot]->obj->step_y = 1;
            enemy_slots[slot]->obj->draw_active = true;
            enemy_slots[slot]->obj->phys_active = true;

            slot = get_enemy_slot();
            enemy_slots[slot]->obj = new enemy_diagonal_stationary_allsprd(eng, enemy_shot_mngr);
            enemy_slots[slot]->obj->init();
            enemy_slots[slot]->obj->pos_x = 630;
            enemy_slots[slot]->obj->pos_y = enemy_slots[slot]->obj->size_y*-1;
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
                    case SDLK_RSHIFT:
                        shield_btn = true;
                        break;
                    case SDLK_p:
                        pause_btn = true;
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
                    case SDLK_RSHIFT:
                        shield_btn = false;
                        break;
                    case SDLK_p:
                        pause_btn = false;
                        break;
                }
        }
    }
}

void init(bool fullscreen)
{
    base_path = SDL_GetBasePath();

#ifdef __EMSCRIPTEN__
    active_enemy_set = -1;
#endif

    chdir(base_path);
    free(base_path);

    eng = new engine("SDL BULLETHELL", RES_X, RES_Y, BPP, fullscreen);
    eng->phys_max_iterations = 20;
    // eng->debug_draw_phys_area = true;

    init_resources();

    // Game objects
    player_shot_mngr = new projectile_manager();
    enemy_shot_mngr = new projectile_manager();
    background_a_obj = new background(eng);
    background_b_obj = new background(eng);
    shield_obj = new shield(eng);
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

    shield_obj->init();
    eng->add_resource("shield_obj", shield_obj);
    eng->add_object(ship_obj);
    ship_obj->init();
    eng->add_object(shield_obj);

    ship_fire_timer = ship_obj->add_timer(AUTO_FIRE_DELAY);

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
    game_over_obj = new class game_over(eng);
    eng->add_object(game_over_obj);
    game_over_obj->init();
    paused_img_obj = new paused_img(eng);
    paused_img_obj->init();
    eng->add_object(paused_img_obj);
    game_ui_obj = new game_ui(eng);
    game_ui_obj->init();
    eng->add_object(game_ui_obj);
}

#ifdef __EMSCRIPTEN__
void menu_loop()
#else
int menu_loop()
#endif
{
    ship_obj->phys_active = false;
    ship_obj->draw_active = false;

    title_obj->draw_active = true;
    press_key_obj->draw_active = true;

#ifndef __EMSCRIPTEN__
    while (true) {
#endif
        get_input();

        if (left | right | up | down | fire) {
            title_obj->draw_active = false;
            press_key_obj->draw_active = false;

#ifdef __EMSCRIPTEN__
            em_game_started = true;
            ship_obj->reset();
            game_ui_obj->draw_active = true;
#else

            return MENU_START;
#endif
        } else if (quit) {
#ifdef __EMSCRIPTEN__
            em_game_started = true;
            ship_obj->reset();
#else
            return MENU_QUIT;
#endif
        }

        eng->step();
#ifndef __EMSCRIPTEN__
    }
#endif
}

void game_loop()
{
    bool init_enemy_set = false;

#ifndef __EMSCRIPTEN__
    active_enemy_set = 0;
    activate_enemy_set(active_enemy_set);

    ship_obj->reset();
    game_ui_obj->draw_active = true;

    while (quit == false) {
#else
    if (quit == false) {
#endif
        get_input();

        if (!game_over) {
            ship_obj->step_x = 0;
            ship_obj->step_y = 0;
            if (left) { ship_obj->step_x = -1; }
            if (right) { ship_obj->step_x = 1; }
            if (up) { ship_obj->step_y = -1; }
            if (down) { ship_obj->step_y = 1; }

            if (pause_btn) {
                if (!pause_pressed) {
                    pause_pressed = true;

                    paused = !paused;

                    if (paused) {
                        paused_img_obj->draw_active = true;
                        eng->suspend_timers();
                    } else {
                        paused_img_obj->draw_active = false;
                        eng->resume_timers();
                    }
                }
            } else {
                pause_pressed = false;
            }

            if (fire) {
                if (!paused && (!fired || ship_fire_timer->tick(eng->timer_now))) {
                    fired = true;
                    ship_fire_timer->last = eng->timer_now;
                    ship_obj->fire();
                }
            } else {
                fired = false;
            }

            if (!paused && shield_btn) {
                ship_obj->activate_shield();
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
        } else {
            game_over_obj->draw_active = true;
        }

        // Redraw screen
        eng->step();
#ifndef __EMSCRIPTEN__
    }
#else
    } else {
        emscripten_cancel_main_loop();
    }
#endif
}

void deinit()
{
    SDL_Quit();

    delete background_a_obj;
    delete background_b_obj;
    delete title_obj;
    delete press_key_obj;
    delete game_over_obj;
    delete paused_img_obj;
    delete ship_obj;
    delete shield_obj;
    delete player_shot_mngr;
    delete enemy_shot_mngr;
    delete powerup_double_shot_obj;
    delete powerup_quad_spread_shot_obj;
    delete game_ui_obj;

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

void help(char *name)
{
    puts("SDL-Bullethell");
    printf("Usage: %s [OPTIONS]\n", name);
    puts(" -h - Show help");
    puts(" -f - Fullscreen mode");
}

#ifdef __EMSCRIPTEN__
void em_loop()
{
    if (em_game_started) {
        game_loop();
    } else {
        menu_loop();
    }
}
#endif

int main(int argc, char *argv[])
{
    int opt;
    bool fullscreen = false;

    while ((opt = getopt(argc, argv, "hf")) != -1) {
        switch (opt) {
            default:
            case 'h':
                help(argv[0]);
                return 0;

            case 'f':
                fullscreen = true;
                break;
        }
    }

    init(fullscreen);

#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(em_loop, 0, 1);
#else
    switch(menu_loop()) {
        case MENU_START:
            game_loop();
            break;
    }
#endif

    deinit();

    return 0;
}

