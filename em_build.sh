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
--preload-file textures \
--preload-file music \
--preload-file sfx
