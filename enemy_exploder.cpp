#include "enemy_exploder.h"
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000

void enemy_exploder::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 36;
    size_y = 29;
    phys_size_x = 36;
    phys_size_y = 29;
    area_y_offset = 29;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_exploder_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_exploder_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 8;
    current_health = 8;

    // shot_delay = 800000000;
    shot_timer = add_timer(800000000);

    initialized = true;
}

void enemy_exploder::init_projectile()
{
    default_shot_texture = NULL;
    default_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
    invincible_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_invincible_anim");
    default_shot_sfx = (Mix_Chunk*)i_eng->get_resource("shot_ball_snd");
}

void enemy_exploder::post_destroy()
{
    int fire_pos_x;
    int fire_pos_y;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            fire_pos_x = x == 1 ? pos_x+size_x : pos_x-18;
            fire_pos_y = y == 1 ? pos_y+size_y : pos_y-15;

            if (x == 0 && y == 0) {
                continue;
            } else if (x == 0) {
                fire_pos_x = pos_x+(size_x/2)-10;
            } else if (y == 0) {
                fire_pos_y = pos_y+(size_y/2)-10;
            }

            p_mngr->fire(
                invincible_shot_animation,
                10,
                10,
                10,
                10,
                fire_pos_x,
                fire_pos_y,
                x,
                y,
                SHOT_PHYS_DELAY*2,
                SHOT_PHYS_DELAY*2,
                true
            );
        }
    }

    Mix_PlayChannel(-1, default_shot_sfx, 0);
}

void enemy_exploder::fire()
{
    p_mngr->fire(
        default_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)-12,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    p_mngr->fire(
        default_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)+7,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    Mix_PlayChannel(-1, default_shot_sfx, 0);
}

enemy_exploder::~enemy_exploder()
{
    if (initialized) {

    }
}