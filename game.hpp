#ifndef KABEUCHI_GAME_HPP_
#define KABEUCHI_GAME_HPP_
#include <iostream>
#include <SDL2/SDL.h>

struct Coord {
    float x;
    float y;
};

class Game {
    public:
        Game();
        ~Game();

        bool Init();
        void RunLoop();
        void Shutdown();

    private:
        SDL_Window* window;
        SDL_Renderer* renderer;

        bool isRunning;
        Uint32 ticksCount;
        int paddleDir;
        Coord paddlePos;
        Coord ballPos;
        Coord ballVel;

        void processInput();
        void updateGame();
        void generateOutput();
};

#endif // KABEUCHI_GAME_HPP_
