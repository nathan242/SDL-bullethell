shooter: shooter.o engine.o
	g++ -g -o shooter shooter.o engine.o `sdl2-config --libs` -lSDL2_image

shooter.o: shooter.cpp
	g++ -g `sdl2-config --cflags` -c shooter.cpp

engine.o: engine.cpp engine.h
	g++ -g `sdl2-config --cflags` -c engine.cpp


