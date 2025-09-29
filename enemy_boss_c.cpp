#include "enemy_boss_c.h"
#include <cmath>
#include "constants.h"

#define ENEMY_SHIP_MOVE_PHYS_DELAY 8000000
#define SHOT_PHYS_DELAY_DEF 10000000
#define SHOT_PHYS_DELAY_DIFF 20000000
#define SHOT_PHYS_DELAY_DIFF_START 100000000

extern engine_obj_list *enemy_slots[];
extern int get_enemy_slot(engine_obj *self = NULL);

void enemy_boss_c::init()
{
    base_enemy::init();

    type_id = ID_ENEMY_SHIP;
    size_x = 200;
    size_y = 181;
    phys_size_x = 162;
    phys_size_y = 60;
    phys_offset_x = 19;
    phys_offset_y = 53;
    area_y_offset = 150;
    move_x = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY);
    move_y = add_timer(ENEMY_SHIP_MOVE_PHYS_DELAY*2);
    bounce = 1;
    default_texture = texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_boss_c_tex");

    hit_texture = (SDL_Texture*)i_eng->get_resource("enemy_ship_boss_c_hit_tex");

    last_hit_timer = add_timer(HIT_FLASH_DELAY);

    default_health = 300;
    current_health = 300;

    shot_timer = add_timer(700000000);
    side_shot_timer = add_timer(900000000);
    far_side_shot_timer = add_timer(1100000000);
    ball_shot_timer_front = add_timer(1800000000);
    ball_shot_timer_back = add_timer(2000000000);

    pos_mid = (size_x/2)-(ship_obj->size_x/2);

    game_ui_obj = (game_ui*)i_eng->get_resource("game_ui_obj");
    game_ui_obj->activate_boss_health(this);

    rear_obj_a = add_proxy();
    rear_obj_a->type_id = type_id;
    rear_obj_a->pos_x = pos_x+46;
    rear_obj_a->pos_y = pos_y+50;
    rear_obj_a->phys_size_x = 108;
    rear_obj_a->phys_size_y = 3;
    rear_obj_a->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_a;

    rear_obj_b = add_proxy();
    rear_obj_b->type_id = type_id;
    rear_obj_b->pos_x = pos_x+8;
    rear_obj_b->pos_y = pos_y+24;
    rear_obj_b->phys_size_x = 26;
    rear_obj_b->phys_size_y = 29;
    rear_obj_b->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_b;

    rear_obj_c = add_proxy();
    rear_obj_c->type_id = type_id;
    rear_obj_c->pos_x = pos_x+166;
    rear_obj_c->pos_y = pos_y+24;
    rear_obj_c->phys_size_x = 26;
    rear_obj_c->phys_size_y = 29;
    rear_obj_c->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_c;

    rear_obj_d = add_proxy();
    rear_obj_d->type_id = type_id;
    rear_obj_d->pos_x = pos_x+50;
    rear_obj_d->pos_y = pos_y;
    rear_obj_d->phys_size_x = 18;
    rear_obj_d->phys_size_y = 50;
    rear_obj_d->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_d;

    rear_obj_e = add_proxy();
    rear_obj_e->type_id = type_id;
    rear_obj_e->pos_x = pos_x+132;
    rear_obj_e->pos_y = pos_y;
    rear_obj_e->phys_size_x = 18;
    rear_obj_e->phys_size_y = 50;
    rear_obj_e->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_e;

    rear_obj_f = add_proxy();
    rear_obj_f->type_id = type_id;
    rear_obj_f->pos_x = pos_x+78;
    rear_obj_f->pos_y = pos_y+14;
    rear_obj_f->phys_size_x = 44;
    rear_obj_f->phys_size_y = 36;
    rear_obj_f->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = rear_obj_f;

    front_obj_a = add_proxy();
    front_obj_a->type_id = type_id;
    front_obj_a->pos_x = pos_x+49;
    front_obj_a->pos_y = pos_y+113;
    front_obj_a->phys_size_x = 102;
    front_obj_a->phys_size_y = 21;
    front_obj_a->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = front_obj_a;

    front_obj_b = add_proxy();
    front_obj_b->type_id = type_id;
    front_obj_b->pos_x = pos_x+29;
    front_obj_b->pos_y = pos_y+119;
    front_obj_b->phys_size_x = 20;
    front_obj_b->phys_size_y = 31;
    front_obj_b->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = front_obj_b;

    front_obj_c = add_proxy();
    front_obj_c->type_id = type_id;
    front_obj_c->pos_x = pos_x+151;
    front_obj_c->pos_y = pos_y+119;
    front_obj_c->phys_size_x = 20;
    front_obj_c->phys_size_y = 31;
    front_obj_c->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = front_obj_c;

    front_obj_d = add_proxy();
    front_obj_d->type_id = type_id;
    front_obj_d->pos_x = pos_x+78;
    front_obj_d->pos_y = pos_y+134;
    front_obj_d->phys_size_x = 44;
    front_obj_d->phys_size_y = 36;
    front_obj_d->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = front_obj_d;

    front_obj_e = add_proxy();
    front_obj_e->type_id = type_id;
    front_obj_e->pos_x = pos_x+94;
    front_obj_e->pos_y = pos_y+170;
    front_obj_e->phys_size_x = 12;
    front_obj_e->phys_size_y = 11;
    front_obj_e->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = front_obj_e;

    side_obj_a = add_proxy();
    side_obj_a->type_id = type_id;
    side_obj_a->pos_x = pos_x;
    side_obj_a->pos_y = pos_y+92;
    side_obj_a->phys_size_x = 19;
    side_obj_a->phys_size_y = 24;
    side_obj_a->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = side_obj_a;

    side_obj_b = add_proxy();
    side_obj_b->type_id = type_id;
    side_obj_b->pos_x = pos_x+181;
    side_obj_b->pos_y = pos_y+92;
    side_obj_b->phys_size_x = 19;
    side_obj_b->phys_size_y = 24;
    side_obj_b->phys_active = true;
    enemy_slots[get_enemy_slot(this)]->obj = side_obj_b;

    initialized = true;
}

void enemy_boss_c::init_projectile()
{
    default_shot_texture = (SDL_Texture*)i_eng->get_resource("projectile_default_tex");
    default_shot_animation = NULL;
    default_shot_sfx = (Mix_Chunk*)i_eng->get_resource("default_player_shot_snd");
    ball_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_anim");
    ball_invincible_shot_animation = (animation_obj*)i_eng->get_resource("projectile_ball_invincible_anim");
    ball_shot_sfx = (Mix_Chunk*)i_eng->get_resource("shot_ball_snd");
    ball_fire_step_x = 1;
    ball_fire_step_y = 1;
    ball_shot_angle = 0;
    ball_shot_angle_diff = 10;
    shot_invincible = false;
}

bool enemy_boss_c::collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y)
{
    bool result = base_enemy::collision_event(obj2, collide_axis, area_x, area_y);

    if (
        obj2 == rear_obj_a
        || obj2 == rear_obj_b
        || obj2 == rear_obj_c
        || obj2 == rear_obj_d
        || obj2 == rear_obj_e
        || obj2 == rear_obj_f
        || obj2 == front_obj_a
        || obj2 == front_obj_b
        || obj2 == front_obj_c
        || obj2 == front_obj_d
        || obj2 == front_obj_e
        || obj2 == side_obj_a
        || obj2 == side_obj_b
    ) {
        result = false;
    }

    return result;
}

void enemy_boss_c::pre_phys_event()
{
    if (pos_y >= 100) {
        step_y = 0;
    }

    step_x = 0;

    if (ship_obj->pos_x > pos_x+pos_mid) {
        step_x = 1;
    } else if (ship_obj->pos_x < pos_x+pos_mid) {
        step_x = -1;
    }

    if (pos_y > (size_y * -1)) {
        if (ball_shot_timer_front->tick(i_eng->timer_now)) {
            ball_fire(100, 90, 20);
        }

        if (ball_shot_timer_back->tick(i_eng->timer_now)) {
            ball_fire(38, 76, 10);
            ball_fire(59, 62, 10);
            ball_fire(140, 62, 10);
            ball_fire(161, 76, 10);
        }

        if (side_shot_timer->tick(i_eng->timer_now)) {
            fire_side();
        }

        if (far_side_shot_timer->tick(i_eng->timer_now)) {
            fire_far_side();
        }
    }

    rear_obj_a->pos_x = pos_x+46;
    rear_obj_a->pos_y = pos_y+50;

    rear_obj_b->pos_x = pos_x+8;
    rear_obj_b->pos_y = pos_y+24;

    rear_obj_c->pos_x = pos_x+166;
    rear_obj_c->pos_y = pos_y+24;

    rear_obj_d->pos_x = pos_x+50;
    rear_obj_d->pos_y = pos_y;

    rear_obj_e->pos_x = pos_x+132;
    rear_obj_e->pos_y = pos_y;

    rear_obj_f->pos_x = pos_x+78;
    rear_obj_f->pos_y = pos_y+14;

    front_obj_a->pos_x = pos_x+49;
    front_obj_a->pos_y = pos_y+113;

    front_obj_b->pos_x = pos_x+29;
    front_obj_b->pos_y = pos_y+119;

    front_obj_c->pos_x = pos_x+151;
    front_obj_c->pos_y = pos_y+119;

    front_obj_d->pos_x = pos_x+78;
    front_obj_d->pos_y = pos_y+134;

    front_obj_e->pos_x = pos_x+94;
    front_obj_e->pos_y = pos_y+170;

    side_obj_a->pos_x = pos_x;
    side_obj_a->pos_y = pos_y+92;

    side_obj_b->pos_x = pos_x+181;
    side_obj_b->pos_y = pos_y+92;

    base_enemy::pre_phys_event();
}

void enemy_boss_c::ball_fire(int offset_x, int offset_y, int size)
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
            shot_invincible ? ball_invincible_shot_animation : ball_shot_animation,
            size,
            size,
            size,
            size,
            pos_x-(size/2)+offset_x,
            pos_y-(size/2)+offset_y,
            ball_fire_step_x,
            ball_fire_step_y,
            move_shot_x_every,
            move_shot_y_every,
            shot_invincible
        );

        shot_invincible = !shot_invincible;
    }

    Mix_PlayChannel(-1, ball_shot_sfx, 0);
}

bool enemy_boss_c::update_fire_step()
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

void enemy_boss_c::fire()
{
    // Front left
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)-13,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front right
    p_mngr->fire(
        default_shot_texture,
        5,
        10,
        5,
        10,
        pos_x+(size_x/2)+8,
        pos_y+size_y+1,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    Mix_PlayChannel(-1, default_shot_sfx, 0);
}

void enemy_boss_c::fire_side()
{
    // Front left ball
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)-27,
        pos_y+size_y-20,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front right ball
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)+17,
        pos_y+size_y-20,
        0,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    Mix_PlayChannel(-1, ball_shot_sfx, 0);
}

void enemy_boss_c::fire_far_side()
{
    // Front far left
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)-79,
        pos_y+size_y-31,
        -1,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    // Front far right
    p_mngr->fire(
        ball_shot_animation,
        10,
        10,
        10,
        10,
        pos_x+(size_x/2)+69,
        pos_y+size_y-31,
        1,
        1,
        SHOT_PHYS_DELAY,
        SHOT_PHYS_DELAY,
        false
    );

    Mix_PlayChannel(-1, ball_shot_sfx, 0);
}

void enemy_boss_c::post_destroy()
{
    rear_obj_a->phys_active = false;
    rear_obj_b->phys_active = false;
    rear_obj_c->phys_active = false;
    rear_obj_d->phys_active = false;
    rear_obj_e->phys_active = false;
    rear_obj_f->phys_active = false;
    front_obj_a->phys_active = false;
    front_obj_b->phys_active = false;
    front_obj_c->phys_active = false;
    front_obj_d->phys_active = false;
    front_obj_e->phys_active = false;
    side_obj_a->phys_active = false;
    side_obj_b->phys_active = false;
}
