#include "enemy_allsprd_attacker.h"
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 12000000

void enemy_allsprd_attacker::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 40;
    size_y = 40;
    phys_size_x = 40;
    phys_size_y = 40;
    area_y_offset = 40;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_allsprd_attacker_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_allsprd_attacker_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 20;
    current_health = 20;

    shot_timer = add_timer(2000000000);

    initialized = true;
}

void enemy_allsprd_attacker::init_projectile()
{
    default_shot_texture = NULL;
    default_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
    default_shot_invincible_animation = (animation_obj*)i_eng->get_resource("projectile_ball_invincible_anim");
    default_shot_sfx = (Mix_Chunk*)i_eng->get_resource("shot_ball_snd");
    ball_fire_step_x = 1;
    ball_fire_step_y = 1;
    ball_shot_angle = 0;
    ball_shot_angle_diff = 10;
    shot_invincible = false;
}

void enemy_allsprd_attacker::post_destroy()
{
    int fire_pos_x;
    int fire_pos_y;

    for (int x = -1; x < 2; x++) {
        for (int y = -1; y < 2; y++) {
            fire_pos_x = x == 1 ? pos_x+size_x : pos_x-20;
            fire_pos_y = y == 1 ? pos_y+size_y : pos_y-20;

            if (x == 0 && y == 0) {
                continue;
            } else if (x == 0) {
                fire_pos_x = pos_x+(size_x/2)-10;
            } else if (y == 0) {
                fire_pos_y = pos_y+(size_y/2)-10;
            }

            p_mngr->fire(
                default_shot_invincible_animation,
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

void enemy_allsprd_attacker::fire()
{
    int move_shot_x_every;
    int move_shot_y_every;
    bool run_loop = true;

    while (run_loop) {
        move_shot_x_every = 9/cos(ball_shot_angle*(M_PI/180))*800000;
        move_shot_y_every = 9/sin(ball_shot_angle*(M_PI/180))*800000;

        ball_shot_angle += ball_shot_angle_diff;

        if (ball_shot_angle > 90 || ball_shot_angle < 0) {
            ball_shot_angle_diff *= -1;
            ball_shot_angle += ball_shot_angle_diff*2;
            run_loop = update_fire_step();
        }

        p_mngr->fire(
            shot_invincible ? default_shot_invincible_animation : default_shot_animation,
            10,
            10,
            10,
            10,
            pos_x+(size_x/2)-5,
            pos_y+(size_y/2)-5,
            ball_fire_step_x,
            ball_fire_step_y,
            move_shot_x_every,
            move_shot_y_every,
            shot_invincible
        );

        shot_invincible = !shot_invincible;
    }

    Mix_PlayChannel(-1, default_shot_sfx, 0);
}

bool enemy_allsprd_attacker::update_fire_step()
{
    if (ball_fire_step_x == 1 && ball_fire_step_y == 1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == 1) {
        ball_fire_step_y *= -1;
    } else if (ball_fire_step_x == -1 && ball_fire_step_y == -1) {
        ball_fire_step_x *= -1;
    } else if (ball_fire_step_x == 1 && ball_fire_step_y == -1) {
        ball_fire_step_y *= -1;
        return false;
    }

    return true;
}

enemy_allsprd_attacker::~enemy_allsprd_attacker()
{
    if (initialized) {

    }
}