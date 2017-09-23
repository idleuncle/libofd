
#.PHONY example0

all: gtkofd

run: gtkofd
	./gtkofd

gtkofd: main.cc sdl_cairo_render.cc 
	gcc -std=c++11 sdl_cairo_render.cc main.cc -o gtkofd `pkg-config --cflags --libs gtk+-3.0` -lSDL2 -lstdc++

clean:
	rm -f *.o gtkofd 
