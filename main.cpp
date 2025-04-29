#include "game.hpp"

Game *game = nullptr;

int main(int argc, char* argv[]){
    const int FPS = 60;
    const int frameDelay = 10;
    int frameStart, frameTime;

    game = new Game();

    game -> init("GAME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, 0);

    while(game -> running()){
        frameStart = SDL_GetTicks();

        game -> gameEvents();

        game -> update();

        frameTime = SDL_GetTicks() - frameStart;

        if(frameDelay > frameTime){
            SDL_Delay(frameDelay - frameTime);
        }

        game -> render();
    }

    game -> clean();

    return 0;
}
