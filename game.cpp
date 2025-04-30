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

    TTF_Init(); loadAll(renderer, volume);
}

void Game::gameEvents(){
    SDL_Event event;
    SDL_PollEvent(&event);

    pos:;

    switch (event.type){
        case SDL_QUIT:
            isRunning = false;
            break;

        case SDL_KEYDOWN:
            if(!isMenu && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE){
                Mix_PauseMusic(); Mix_Pause(-1);
                SDL_RenderCopy(renderer, pause, NULL, NULL);
                SDL_RenderPresent(renderer);

                int x, y;

                while(1){
                    SDL_Event e;
                    SDL_PollEvent(&e);

                    if(e.type == SDL_QUIT){
                        isRunning = 0;
                        break;
                    }

                    if(e.type == SDL_MOUSEBUTTONDOWN){
                        SDL_GetMouseState(&x, &y);
                        SDL_Rect mouse = {x, y, 0, 0};

                        if(intersect(mouse, resume_rect)){
                            Mix_ResumeMusic(); Mix_Resume(-1);
                            play(menu_sound); break;
                        }

                        if(intersect(mouse, menu_rect)){
                            play(menu_sound);
                            isMenu = 1; break;
                        }
                    }
                }
            }
        default:
            if(isMenu){
                SDL_RenderCopy(renderer, menu, NULL, NULL);
                SDL_RenderPresent(renderer);
                int x, y;

                while(1){
                    SDL_Event e;
                    SDL_PollEvent(&e);

                    if(e.type == SDL_QUIT){
                        isRunning = 0; 
                        break;
                    }

                    if(e.type == SDL_MOUSEBUTTONDOWN){
                        SDL_GetMouseState(&x, &y);
                        SDL_Rect mouse = {x, y, 0, 0};

                        if(intersect(mouse, resume_rect)){
                            reset(); isMenu = 0; play(menu_sound);
                            play(music); break;
                        }

                        if(intersect(mouse, menu_rect)){
                            play(menu_sound);
                            Mix_VolumeMusic(volume);
                            Mix_Volume(-1, volume);

                            SDL_Rect return_rect = {230, 450, 330, 90};
                            SDL_Rect sliderBar = {250+100, 200-25, 300, 10};
                            SDL_Rect sliderKnob = {250+100 + (volume * 300 / 128) - 5, 195-25, 10, 20};
                            SDL_Rect sliderBar2 = {250+100, 200-25 + 90, 300, 10};
                            SDL_Rect sliderKnob2 = {250+100 + (((120 - gameSpeed) * 300) / 128) - 5, 195-25 + 90, 10, 20};

                            SDL_Event e;
                            bool flag = 0, flag2 = 0;

                            while(1){
                                while(SDL_PollEvent(&e)){
                                    if(e.type == SDL_QUIT){
                                        isRunning = 0;
                                        goto gello;
                                    }

                                    if(e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEMOTION){
                                        if(e.button.button == SDL_BUTTON_LEFT){
                                            int mx = e.button.x, my = e.button.y;
                                            if(((flag && !flag2) || (my >= sliderBar.y - 10 && my <= sliderBar.y + sliderBar.h + 10))
                                             && mx >= sliderBar.x && mx <= sliderBar.x + sliderBar.w) {
                                                volume = (mx - sliderBar.x) * 128 / sliderBar.w;
                                                Mix_VolumeMusic(volume); Mix_Volume(-1, volume);
                                                sliderKnob.x = mx - 5; flag = 1;
                                            }

                                            if(((flag2 && !flag) || (my >= sliderBar2.y - 10 && my <= sliderBar2.y + sliderBar2.h + 10))
                                             && mx >= sliderBar2.x && mx <= sliderBar2.x + sliderBar2.w) {
                                                gameSpeed = (sliderBar2.x + sliderBar2.w - mx) * 128 / sliderBar2.w;
                                                sliderKnob2.x = mx - 5; flag2 = 1;
                                            }
                                        }
                                    }
                                    gameSpeed = max(gameSpeed, 1);
                                    if(e.type == SDL_MOUSEBUTTONUP) flag2 = 0, flag = 0;

                                    if(e.type == SDL_MOUSEBUTTONDOWN){
                                        SDL_GetMouseState(&x, &y);
                                        SDL_Rect mouse = {x, y, 0, 0};

                                        if(intersect(mouse, return_rect)){
                                            Mix_ResumeMusic(); Mix_Resume(-1);
                                            play(menu_sound); goto pos;
                                        }
                                    }
                                }
                                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                                SDL_RenderCopy(renderer, settings, NULL, NULL);
                                SDL_RenderFillRect(renderer, &sliderBar2);
                                SDL_RenderFillRect(renderer, &sliderBar);
                                SDL_SetRenderDrawColor(renderer, 200, 50, 50, 255);
                                SDL_RenderFillRect(renderer, &sliderKnob2);
                                SDL_RenderFillRect(renderer, &sliderKnob);
                                SDL_RenderPresent(renderer);
                                SDL_RenderClear(renderer);
                            }
                        }
                    }
                }
            }
            break;
    }

    gello:;
}

void Game::render(){
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    upd_hb_and_time(); add_chest();
    kill_ghost(); open_chest(); ghost_attack();

    cnt++; cdr(); collision_detection();
    ghost_control(); draw_effect();

    if(hp <= 0) return game_over(best, isRunning, isMenu);

    SDL_RenderPresent(renderer);

    if(cnt % (2 - (dSpeed > 0)) == 0) t++;
    if(cnt % gameSpeed == 0) score++;
}

void Game::clean(){
    SDL_RenderClear(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
