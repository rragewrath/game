#pragma once

#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>

const double land = 400;

class Game {
    public:
        Game();
        ~Game();

        void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

        void gameEvents();
        void update();
        void render();
        void start();
        void clean();

        bool running(){
            return isRunning;
        }

    private:
        bool isRunning;
        int best;
};
