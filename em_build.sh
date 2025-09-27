#!/bin/bash

em++ -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_MIXER=2 -s SDL2_MIXER_FORMATS=["ogg"] \
bullethell.cpp \
engine.cpp \
enemy.cpp \
base_enemy.cpp \
background.cpp \
base_projectile.cpp \
enemy_adv.cpp \
enemy_adv_spread.cpp \
enemy_boss_a.cpp \
enemy_boss_b.cpp \
enemy_boss_c.cpp \
enemy_cargo.cpp \
enemy_diagonal.cpp \
enemy_diagonal_stationary.cpp \
enemy_diagonal_stationary_spiral.cpp \
enemy_diagonal_stationary_allsprd.cpp \
enemy_diagonal_stationary_fwdsprd.cpp \
enemy_exploder.cpp \
enemy_allsprd_attacker.cpp \
enemy_projectile.cpp \
menu_title.cpp \
player_projectile.cpp \
powerup.cpp \
powerup_double_shot.cpp \
powerup_quad_spread_shot.cpp \
press_key.cpp \
game_over.cpp \
paused_img.cpp \
complete.cpp \
anim_projectile_ball.cpp \
anim_projectile_ball_invincible.cpp \
anim_powerup_double_shot.cpp \
anim_powerup_quad_spread_shot.cpp \
anim_explosion.cpp \
projectile_manager.cpp \
ship.cpp \
shield.cpp \
game_ui.cpp \
explosion.cpp \
explosion_manager.cpp \
level_text.cpp \
-o bullethell.html \
--preload-file background_1.png \
--preload-file background_2.png \
--preload-file background_3.png \
--preload-file enemy_cargo_hit.png \
--preload-file enemy_cargo.png \
--preload-file enemy_ship_adv_hit.png \
--preload-file enemy_ship_adv.png \
--preload-file enemy_ship_adv_spread_hit.png \
--preload-file enemy_ship_adv_spread.png \
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
--preload-file enemy_ship_boss_a.png \
--preload-file enemy_ship_boss_a_hit.png \
--preload-file enemy_ship_boss_b.png \
--preload-file enemy_ship_boss_b_hit.png \
--preload-file enemy_ship_boss_c.png \
--preload-file enemy_ship_boss_c_hit.png \
--preload-file enemy_ship_allsprd_attacker.png \
--preload-file enemy_ship_allsprd_attacker_hit.png \
--preload-file enemy_ship_exploder.png \
--preload-file enemy_ship_exploder_hit.png \
--preload-file press_a_key.png \
--preload-file game_over.png \
--preload-file paused.png \
--preload-file complete.png \
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
--preload-file boss_health.png \
--preload-file powerup_single_shot.png \
--preload-file green_bar.png \
--preload-file red_bar.png \
--preload-file explosion_1.png \
--preload-file explosion_2.png \
--preload-file explosion_3.png \
--preload-file explosion_4.png \
--preload-file level_text.png \
--preload-file 1.png \
--preload-file 2.png \
--preload-file 3.png \
--preload-file music
