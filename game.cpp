#include "game.hpp"
using namespace std;
mt19937 rd(time(NULL));
#include "header.hpp"
#include "object.hpp"
#include "movement.hpp"
#include "graphics.hpp"

Game::Game(){}
Game::~Game(){}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    ifstream inp("best.txt");
    inp >> best; inp.close();

    int flag = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if(!SDL_Init(SDL_INIT_EVERYTHING)){
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);
        renderer = SDL_CreateRenderer(window, -1, 0);
        isRunning = true;
    } else isRunning = false;

    TTF_Init(); loadAll(renderer);
}

void Game::gameEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                Mix_PauseMusic();
                SDL_RenderCopy(renderer, resume, NULL, NULL);
                SDL_RenderPresent(renderer);

                while(1){
                    SDL_Event e;
                    SDL_PollEvent(&e);

                    if(e.type == SDL_KEYDOWN && e.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                        Mix_ResumeMusic();
                        break;
                    }
                }
            }
            break;

        default:
            break;
    }
}

void Game::start(){
    SDL_RenderCopy(renderer, st, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_Delay(1000);

    while(1){
        SDL_Event event;
        SDL_PollEvent(&event);
        switch (event.type){
            case SDL_QUIT:
                isRunning = false;
                return;
                break;

            case SDL_KEYDOWN:
                play(music);
                return;
                break;

            default:
                break;
        }
    }
}

void Game::render(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    upd_hb_and_time();
    add_chest();
    kill_ghost();
    open_chest();
    remove_chest();

    cnt++; cnt %= FPS; cdr();
    collision_detection();
    ghost_control();
    draw_effect();

    if(hp <= 0){
        return game_over(best, isRunning);
    }

    SDL_RenderPresent(renderer);

    if(cnt % (2 - (dSpeed > 0)) == 0) t++;
    if(cnt == 0) score++;
    t %= FPS;
}

void Game::clean(){
    SDL_RenderClear(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
