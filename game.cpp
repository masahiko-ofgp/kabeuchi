#include "game.hpp"

const int THICKNESS = 15;
const float PADDLE_H = 100.0f;

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    ticksCount = 0;
    isRunning = true;
    paddleDir = 0;
}

Game::~Game() {}

bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Unabled to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(
            "Step1 Game",
            100,
            100,
            600,
            600,
            0
    );

    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(
            window,
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    paddlePos.x = 10.0f;
    paddlePos.y = 600.0f / 2.0f;
    ballPos.x = 600.0f / 2.0f;
    ballPos.y = 600.0f / 2.0f;
    ballVel.x = -200.0f;
    ballVel.y = 235.0f;
    return true;
}

void Game::RunLoop() {
    while (isRunning) {
        processInput();
        updateGame();
        generateOutput();
    }
}

void Game::processInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                isRunning = false;
                break;
        }
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    if (state[SDL_SCANCODE_ESCAPE]) {
        isRunning = false;
    }

    paddleDir = 0;
    if (state[SDL_SCANCODE_W]) {
        paddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S]) {
        paddleDir += 1;
    }
}

void Game::updateGame() {
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16))
        ;

    float deltaTime = (SDL_GetTicks() - ticksCount) / 1000.0f;

    if (deltaTime > 0.05f) {
        deltaTime = 0.05f;
    }

    ticksCount = SDL_GetTicks();

    if (paddleDir != 0) {
        paddlePos.y += paddleDir * 300.0f * deltaTime;

        if (paddlePos.y < (PADDLE_H/2.0f + THICKNESS)) {
            paddlePos.y = PADDLE_H/2.0f + THICKNESS;
        }
        else if (paddlePos.y > (600.0f - PADDLE_H/2.0f - THICKNESS)) {
            paddlePos.y = 600.0f - PADDLE_H/2.0f - THICKNESS;
        }
    }

    ballPos.x += ballVel.x * deltaTime;
    ballPos.y += ballVel.y * deltaTime;

    float diff = paddlePos.y - ballPos.y;

    diff = (diff > 0.0f) ? diff : -diff;
    if (diff <= PADDLE_H / 2.0f &&
        ballPos.x <= 25.0f &&
        ballPos.x >= 20.0f &&
        ballVel.x < 0.0f) {
        ballVel.x *= -1.0f;
    }
    else if (ballPos.x <= 0.0f) {
        isRunning = false;
    }
    else if (ballPos.x >= (600.0f - THICKNESS) && ballVel.x > 0.0f) {
        ballVel.x *= -1.0f;
    }

    if (ballPos.y <= THICKNESS && ballVel.y < 0.0f) {
        ballVel.y *= -1;
    }
    else if (ballPos.y >= (600 - THICKNESS) && ballVel.y > 0.0f) {
        ballVel.y *= -1;
    }
}

void Game::generateOutput() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect wall {0, 0, 600, THICKNESS};

    SDL_RenderFillRect(renderer, &wall);

    wall.y = 600 - THICKNESS;
    SDL_RenderFillRect(renderer, &wall);

    wall.x = 600 - THICKNESS;
    wall.y = 0;
    wall.w = THICKNESS;
    wall.h = 600;
    SDL_RenderFillRect(renderer, &wall);

    SDL_Rect paddle {
        static_cast<int>(paddlePos.x),
        static_cast<int>(paddlePos.y - PADDLE_H/2),
        THICKNESS,
        static_cast<int>(PADDLE_H)
    };
    SDL_RenderFillRect(renderer, &paddle);

    SDL_Rect ball {
        static_cast<int>(ballPos.x - THICKNESS/2),
        static_cast<int>(ballPos.y - THICKNESS/2),
        THICKNESS,
        THICKNESS
    };
    SDL_RenderFillRect(renderer, &ball);

    SDL_RenderPresent(renderer);
}

void Game::Shutdown() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
