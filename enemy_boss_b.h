#ifndef ENEMY_BOSS_B_H
#define ENEMY_BOSS_B_H

#include "base_enemy.h"
#include "game_ui.h"

class enemy_boss_b : public base_enemy
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
        Mix_Chunk *ball_shot_sfx;
        game_ui *game_ui_obj;
        proxy_engine_obj* rear_obj_a;
        proxy_engine_obj* rear_obj_b;
        proxy_engine_obj* front_left_gun_obj;
        proxy_engine_obj* front_right_gun_obj;
        proxy_engine_obj* front_obj;
        bool update_fire_step();
        void init_projectile();
        void pre_phys_event();
        void post_destroy();

    public:
        void init();
        bool collision_event(engine_obj *obj2, int collide_axis, int area_x, int area_y);
        void ball_fire(int offset_y);
        void fire();
        void fire_side();
        void fire_far_side();
};

#endif
