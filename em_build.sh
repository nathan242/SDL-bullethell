#!/bin/bash

em++ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] \
bullethell.cpp \
engine.cpp \
enemy.cpp \
base_enemy.cpp \
background.cpp \
base_projectile.cpp \
enemy_adv.cpp \
enemy_boss_a.cpp \
enemy_cargo.cpp \
enemy_diagonal.cpp \
enemy_diagonal_stationary.cpp \
enemy_diagonal_stationary_spiral.cpp \
enemy_diagonal_stationary_allsprd.cpp \
enemy_diagonal_stationary_fwdsprd.cpp \
enemy_projectile.cpp \
menu_title.cpp \
player_projectile.cpp \
powerup.cpp \
powerup_double_shot.cpp \
powerup_quad_spread_shot.cpp \
press_key.cpp \
game_over.cpp \
paused_img.cpp \
anim_projectile_ball.cpp \
anim_projectile_ball_invincible.cpp \
anim_powerup_double_shot.cpp \
anim_powerup_quad_spread_shot.cpp \
projectile_manager.cpp \
ship.cpp \
shield.cpp \
game_ui.cpp \
-o bullethell.html \
--preload-file background.png \
--preload-file enemy_cargo_hit.png \
--preload-file enemy_cargo.png \
--preload-file enemy_ship_adv_hit.png \
--preload-file enemy_ship_adv.png \
--preload-file enemy_ship_default_hit.png \
--preload-file enemy_ship_default.png \
--preload-file enemy_ship_diagonal_6_hit.png \
--preload-file enemy_ship_diagonal_6.png \
--preload-file enemy_ship_diagonal_hit.png \
--preload-file enemy_ship_diagonal.png \
--preload-file powerup_double_shot.png \
--preload-file powerup_double_shot_1.png \
--preload-file powerup_double_shot_2.png \
--preload-file powerup_quad_spread_shot.png \
--preload-file powerup_quad_spread_shot_1.png \
--preload-file powerup_quad_spread_shot_2.png \
--preload-file press_a_key.png \
--preload-file game_over.png \
--preload-file paused.png \
--preload-file projectile_ball_invincible.png \
--preload-file projectile_ball_invincible_1.png \
--preload-file projectile_ball_invincible_2.png \
--preload-file projectile_ball_invincible_3.png \
--preload-file projectile_ball_invincible_4.png \
--preload-file projectile_ball_invincible_5.png \
--preload-file projectile_ball.png \
--preload-file projectile_ball_1.png \
--preload-file projectile_ball_2.png \
--preload-file projectile_ball_3.png \
--preload-file projectile_ball_4.png \
--preload-file projectile_ball_5.png \
--preload-file projectile_default.png \
--preload-file projectile_player_default.png \
--preload-file ship.png \
--preload-file title.png \
--preload-file shield.png \
--preload-file game_ui_bar.png \
--preload-file powerup_single_shot.png \
--preload-file green_bar.png \
--preload-file red_bar.png
