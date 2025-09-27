#ifndef ENEMY_BOSS_C_H
#define ENEMY_BOSS_C_H

#include "base_enemy.h"
#include "game_ui.h"

class enemy_boss_c : public base_enemy
{
    using base_enemy::base_enemy;

    protected:
        int ball_fire_step_x;
        int ball_fire_step_y;
        int ball_shot_angle;
        int ball_shot_angle_diff;
        int pos_mid;
        bool shot_invincible;
        timer_obj *side_shot_timer;
        timer_obj *far_side_shot_timer;
        timer_obj *ball_shot_timer_front;
        timer_obj *ball_shot_timer_back;
        animation_obj *ball_shot_animation;
        animation_obj *ball_invincible_shot_animation;
        game_ui *game_ui_obj;
        proxy_engine_obj* rear_obj_a;
        proxy_engine_obj* rear_obj_b;
        proxy_engine_obj* rear_obj_c;
        proxy_engine_obj* rear_obj_d;
        proxy_engine_obj* rear_obj_e;
        proxy_engine_obj* rear_obj_f;
        proxy_engine_obj* front_obj_a;
        proxy_engine_obj* front_obj_b;
        proxy_engine_obj* front_obj_c;
        proxy_engine_obj* front_obj_d;
        proxy_engine_obj* front_obj_e;
        proxy_engine_obj* side_obj_a;
        proxy_engine_obj* side_obj_b;
        bool update_fire_step();
        void init_projectile();
        void pre_phys_event();
        void post_destroy();

    public:
        void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void ball_fire(int offset_x, int offset_y, int size);
        void fire();
        void fire_side();
        void fire_far_side();
};

#endif
