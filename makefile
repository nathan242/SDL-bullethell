shooter: shooter.o engine.o ship.o enemy.o enemy_diagonal.o enemy_diagonal_stationary.o player_projectile.o enemy_projectile.o projectile_manager.o
	g++ -g -o shooter shooter.o engine.o ship.o enemy.o enemy_diagonal.o enemy_diagonal_stationary.o player_projectile.o enemy_projectile.o projectile_manager.o `sdl2-config --libs` -lSDL2_image

shooter.o: shooter.cpp
	g++ -g `sdl2-config --cflags` -c shooter.cpp

engine.o: engine.cpp engine.h
	g++ -g `sdl2-config --cflags` -c engine.cpp

ship.o: ship.cpp ship.h
	g++ -g `sdl2-config --cflags` -c ship.cpp

enemy.o: enemy.cpp enemy.h
	g++ -g `sdl2-config --cflags` -c enemy.cpp

enemy_diagonal.o: enemy_diagonal.cpp enemy_diagonal.h
	g++ -g `sdl2-config --cflags` -c enemy_diagonal.cpp

enemy_diagonal_stationary.o: enemy_diagonal_stationary.cpp enemy_diagonal_stationary.h
	g++ -g `sdl2-config --cflags` -c enemy_diagonal_stationary.cpp

player_projectile.o: player_projectile.cpp player_projectile.h
	g++ -g `sdl2-config --cflags` -c player_projectile.cpp

enemy_projectile.o: enemy_projectile.cpp enemy_projectile.h
	g++ -g `sdl2-config --cflags` -c enemy_projectile.cpp

projectile_manager.o: projectile_manager.cpp projectile_manager.h
	g++ -g `sdl2-config --cflags` -c projectile_manager.cpp
