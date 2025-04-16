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

    if(book_t == 14) cast = book_t = 0;
    if(cast && cnt % 3 == 0) book_t++;

    if(cast){
        skillhb.x = xPlayer + 90 - 105 * (isLeft);
        skillhb.y = yPlayer;
        skillhb.h = 20 + book_t * 5;
        skillhb.w = 20;
    }

    hb.h = 58 + (dShield > 0) * 20;
    hb.w = 33 + (dShield > 0) * 20;

    add_chest();
    kill_ghost();
    open_chest();
    remove_chest();

    cnt++; cnt %= FPS; cdr();

    SDL_SetRenderDrawColor(renderer, 137, 214, 205, 255);

    if(wall.w > 0){
        if(cnt % 16 == 1) wall.w -= 2, wall.x++;
        SDL_RenderFillRect(renderer, &wall);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if(chest) renderTexture(Chest, xChest, land, renderer);

    while(q.size()){
        auto u = q.back();
        q.pop_back();

        if(u.y - land - 20){
            SDL_Rect sw;

            if(!u.t){
                sw.x = u.x+9; sw.y = u.y;
                sw.h = 77; sw.w = 28;
            } else {
                sw.x = u.x + 6; sw.y = u.y + 15;
                sw.h = 20; sw.w = 49;
            }

            if(intersect(hb, sw)){
                if(dShield <= 0){
                    p.push_back({xPlayer + 25, yPlayer - 20, 0.2, harm});
                    play(harmSound);
                    hp--;
                } else {
                    p.push_back({xPlayer + 25, yPlayer - 20, 0.2, shielded});
                    play(block);
                }

                continue;
            }

            u.upd();

            if(!intersect(sw, wall) || wall.w <= 0) tmp.push_back(u);

            if(!u.t) renderTexture(sword, u.x, u.y, renderer);
                else renderTexture(sword2, u.x, u.y, renderer);
        }
    }

    swap(q, tmp);
    ghost_control(renderer);
    draw_effect(renderer);

    while(p.size()){
        auto u = p.back();
        p.pop_back();

        if(u.d > 0){
            renderTexture(u.image, u.x, u.y, renderer);
            u.d -= 1.0 / FPS;
            tmp2.push_back(u);
        }
    }

    swap(p, tmp2);

    if(hp <= 0){
        SDL_RenderClear(renderer);

        SDL_Event events;
        Mix_PauseMusic();
        play(gameOver);

        while(true){
            SDL_Texture* background = loadTexture("images/over.jpg", renderer);
            SDL_Texture* replay = loadTexture("images/replay.png", renderer);
            SDL_RenderCopy(renderer, background, NULL, NULL);

            best = max(best, score);

            char const *p1 = ("Score: " + to_string(score)).c_str();
            char const *p2 = ("Best: " + to_string(best)).c_str();

            SDL_Texture* SCORE = renderText(p1, font, {236, 221, 34, 255}, renderer);
            SDL_Texture* HIGH = renderText(p2, font, {236, 221, 34, 255}, renderer);

            int tW, tH;

            SDL_QueryTexture(SCORE, NULL, NULL, &tW, &tH);

            SDL_Rect textRect = {(800 - tW) / 2, 420, tW, tH};
            SDL_Rect textRECT = {(800 - tW) / 2, 515, tW, tH};

            SDL_RenderCopy(renderer, SCORE, NULL, &textRect);
            SDL_RenderCopy(renderer, HIGH, NULL, &textRECT);

            renderTexture(replay, 276, 318 - 30, renderer);

            SDL_DestroyTexture(background); background = NULL;
            SDL_DestroyTexture(replay); replay = NULL;
            SDL_DestroyTexture(SCORE); SCORE = NULL;

            SDL_Rect rep;

            rep.x = 293+14;
            rep.y = 363+12 - 30;
            rep.w = 171;
            rep.h = 68;

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderPresent(renderer);

            int x, y;

            SDL_PollEvent(&events);
            SDL_GetMouseState(&x, &y);

            if(events.type == SDL_MOUSEBUTTONDOWN){
                SDL_Rect mouse;
                mouse.x = x; mouse.y = y;
                mouse.h = 0; mouse.w = 0;

                if(intersect(mouse, rep)){
                    reset();
                    break;
                }
            } else if(events.type == SDL_QUIT){
                ofstream out("best.txt");

                out << best;
                out.close();

                isRunning = 0;
                break;
            }
        }

        return;
    }

    SDL_RenderPresent(renderer);

    if(cnt % (2 - (dSpeed > 0)) == 0) t++;
    t %= FPS;
}

void Game::clean(){
    SDL_RenderClear(renderer);
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
