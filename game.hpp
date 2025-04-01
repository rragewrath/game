#ifndef Game_hpp
#define Game_hpp

#include<SDL.h>
#include<SDL_image.h>
#include<bits/stdc++.h>
#include <SDL_mixer.h>
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
    //800, 600
    private:
        bool isRunning;
        SDL_Window *window;
        SDL_Renderer *renderer;
        double xPlayer = 0;
        double yPlayer = land;
        int score = 0, best;

        SDL_Texture *background;
        SDL_Texture *resume;
        SDL_Texture *st;

        Mix_Chunk *harmSound;
        Mix_Chunk *gameOver;
        Mix_Chunk *dashSound;
        Mix_Chunk *skillSound;
        Mix_Chunk *windwallSound;
        Mix_Chunk *takescoreSound;
        Mix_Chunk *CD;
        Mix_Chunk *heal;
        Mix_Chunk *speedboost;
        Mix_Chunk *jump;
        Mix_Chunk *block;

        Mix_Music *music;

        SDL_Texture *walk[11][2];
        SDL_Texture *book[13];
        SDL_Texture *b[13];
        SDL_Texture *skill[13];
        SDL_Texture *sword;
        SDL_Texture *sword2;
        SDL_Texture *cd;
        SDL_Texture *angry1;
        SDL_Texture *angry2;
        SDL_Texture *blood;
        SDL_Texture *ghost1;
        SDL_Texture *ghost2;
        SDL_Texture *dash_particle;
        SDL_Texture *dash;
        SDL_Texture *killangry;
        SDL_Texture *windwall;
        SDL_Texture *shielded;
        SDL_Texture *killghost;
        SDL_Texture *Chest;
        SDL_Texture *harm;
        SDL_Texture *shield;
        SDL_Texture *heart;
        SDL_Texture *heart2;
        SDL_Texture *takescore;
        SDL_Texture *speed;
        SDL_Texture *speed2;
        SDL_Texture *HP;
        TTF_Font *font;
};

#endif // Game_hpp
