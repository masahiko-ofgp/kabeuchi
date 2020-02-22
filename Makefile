CC := clang++

FLAGS := `sdl2-config --libs --cflags` -ggdb -O0 --std=c++17 -Weverything -lSDL2

main: main.cpp game.cpp game.hpp
	$(CC) -c game.cpp -o game.o
	$(CC) -c main.cpp -o main.o
	$(CC) -o $@ main.o game.o $(FLAGS)

clean:
	rm -f *.o main
PHONY: clean
