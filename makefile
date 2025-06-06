bullethell: bullethell.o \
engine.o \
background.o \
menu_title.o \
press_key.o \
game_over.o \
paused_img.o \
game_ui.o \
anim_projectile_ball.o \
anim_projectile_ball_invincible.o \
anim_powerup_double_shot.o \
anim_powerup_quad_spread_shot.o \
ship.o \
shield.o \
enemy.o \
base_enemy.o \
enemy_adv.o \
enemy_diagonal.o \
enemy_diagonal_stationary.o \
enemy_diagonal_stationary_spiral.o \
enemy_diagonal_stationary_allsprd.o \
enemy_diagonal_stationary_fwdsprd.o \
enemy_cargo.o \
enemy_boss_a.o \
powerup.o \
powerup_double_shot.o \
powerup_quad_spread_shot.o \
base_projectile.o \
player_projectile.o \
enemy_projectile.o \
projectile_manager.o
	g++ -Wall -g -o bullethell \
	bullethell.o \
	engine.o \
	background.o \
	menu_title.o \
	press_key.o \
	game_over.o \
	paused_img.o \
	game_ui.o \
	anim_projectile_ball.o \
	anim_projectile_ball_invincible.o \
	anim_powerup_double_shot.o \
	anim_powerup_quad_spread_shot.o \
	ship.o \
	shield.o \
	enemy.o \
	base_enemy.o \
	enemy_adv.o \
	enemy_diagonal.o \
	enemy_diagonal_stationary.o \
	enemy_diagonal_stationary_spiral.o \
	enemy_diagonal_stationary_allsprd.o \
	enemy_diagonal_stationary_fwdsprd.o \
	enemy_cargo.o \
	enemy_boss_a.o \
	powerup.o \
	powerup_double_shot.o \
	powerup_quad_spread_shot.o \
	base_projectile.o \
	player_projectile.o \
	enemy_projectile.o \
	projectile_manager.o \
	`sdl2-config --libs` -lSDL2_image

bullethell.o: bullethell.cpp
	g++ -Wall -g `sdl2-config --cflags` -c bullethell.cpp

engine.o: engine.cpp engine.h
	g++ -Wall -g `sdl2-config --cflags` -c engine.cpp

background.o: background.cpp background.h
	g++ -Wall -g `sdl2-config --cflags` -c background.cpp

menu_title.o: menu_title.cpp menu_title.h
	g++ -Wall -g `sdl2-config --cflags` -c menu_title.cpp

press_key.o: press_key.cpp press_key.h
	g++ -Wall -g `sdl2-config --cflags` -c press_key.cpp

game_over.o: game_over.cpp game_over.h
	g++ -Wall -g `sdl2-config --cflags` -c game_over.cpp

paused_img.o: paused_img.cpp paused_img.h
	g++ -Wall -g `sdl2-config --cflags` -c paused_img.cpp

game_ui.o: game_ui.cpp game_ui.h
	g++ -Wall -g `sdl2-config --cflags` -c game_ui.cpp

anim_projectile_ball.o: anim_projectile_ball.cpp anim_projectile_ball.h
	g++ -Wall -g `sdl2-config --cflags` -c anim_projectile_ball.cpp

anim_projectile_ball_invincible.o: anim_projectile_ball_invincible.cpp anim_projectile_ball_invincible.h
	g++ -Wall -g `sdl2-config --cflags` -c anim_projectile_ball_invincible.cpp

anim_powerup_double_shot.o: anim_powerup_double_shot.cpp anim_powerup_double_shot.h
	g++ -Wall -g `sdl2-config --cflags` -c anim_powerup_double_shot.cpp

anim_powerup_quad_spread_shot.o: anim_powerup_quad_spread_shot.cpp anim_powerup_quad_spread_shot.h
	g++ -Wall -g `sdl2-config --cflags` -c anim_powerup_quad_spread_shot.cpp

ship.o: ship.cpp ship.h
	g++ -Wall -g `sdl2-config --cflags` -c ship.cpp

shield.o: shield.cpp shield.h
	g++ -Wall -g `sdl2-config --cflags` -c shield.cpp

base_enemy.o: base_enemy.cpp base_enemy.h
	g++ -Wall -g `sdl2-config --cflags` -c base_enemy.cpp

enemy.o: enemy.cpp enemy.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy.cpp

enemy_adv.o: enemy_adv.cpp enemy_adv.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_adv.cpp

enemy_diagonal.o: enemy_diagonal.cpp enemy_diagonal.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_diagonal.cpp

enemy_diagonal_stationary.o: enemy_diagonal_stationary.cpp enemy_diagonal_stationary.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_diagonal_stationary.cpp

enemy_diagonal_stationary_spiral.o: enemy_diagonal_stationary_spiral.cpp enemy_diagonal_stationary_spiral.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_diagonal_stationary_spiral.cpp

enemy_diagonal_stationary_allsprd.o: enemy_diagonal_stationary_allsprd.cpp enemy_diagonal_stationary_allsprd.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_diagonal_stationary_allsprd.cpp

enemy_diagonal_stationary_fwdsprd.o: enemy_diagonal_stationary_fwdsprd.cpp enemy_diagonal_stationary_fwdsprd.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_diagonal_stationary_fwdsprd.cpp

enemy_cargo.o: enemy_cargo.cpp enemy_cargo.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_cargo.cpp

enemy_boss_a.o: enemy_boss_a.cpp enemy_boss_a.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_boss_a.cpp

powerup.o: powerup.cpp powerup.h
	g++ -Wall -g `sdl2-config --cflags` -c powerup.cpp

powerup_double_shot.o: powerup_double_shot.cpp powerup_double_shot.h
	g++ -Wall -g `sdl2-config --cflags` -c powerup_double_shot.cpp

powerup_quad_spread_shot.o: powerup_quad_spread_shot.cpp powerup_quad_spread_shot.h
	g++ -Wall -g `sdl2-config --cflags` -c powerup_quad_spread_shot.cpp

base_projectile.o: base_projectile.cpp base_projectile.h
	g++ -Wall -g `sdl2-config --cflags` -c base_projectile.cpp

player_projectile.o: player_projectile.cpp player_projectile.h
	g++ -Wall -g `sdl2-config --cflags` -c player_projectile.cpp

enemy_projectile.o: enemy_projectile.cpp enemy_projectile.h
	g++ -Wall -g `sdl2-config --cflags` -c enemy_projectile.cpp

projectile_manager.o: projectile_manager.cpp projectile_manager.h
	g++ -Wall -g `sdl2-config --cflags` -c projectile_manager.cpp
