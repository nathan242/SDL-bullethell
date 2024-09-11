shooter: shooter.o physics.o graphics.o
	g++ -g -o shooter shooter.o physics.o graphics.o `sdl2-config --libs` -lSDL2_image

shooter.o: shooter.cpp
	g++ -g `sdl2-config --cflags` -c shooter.cpp

physics.o: physics.cpp physics.h
	g++ -g `sdl2-config --cflags` -c physics.cpp

graphics.o: graphics.cpp graphics.h
	g++ -g `sdl2-config --cflags` -c graphics.cpp

