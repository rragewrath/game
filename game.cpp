#include "game.hpp"
using namespace std;

Game::Game(){}

Game::~Game(){}

SDL_Texture* renderText(const char* text, TTF_Font* font, SDL_Color textColor, SDL_Renderer* renderer){
    SDL_Surface* textSurface =
            TTF_RenderText_Solid(font, text, textColor);
    if(textSurface == nullptr) return nullptr;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    return texture;
}

SDL_Texture *loadTexture(const char *filename, SDL_Renderer* renderer){
	SDL_Texture *texture = IMG_LoadTexture(renderer, filename);
	return texture;
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer){
    IMG_Quit();
}

void renderTexture(SDL_Texture *texture, double x, double y,
                   SDL_Renderer* renderer){
	SDL_Rect dest;
	dest.x = x;
	dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

TTF_Font* loadFont(const char* path, int size){
    TTF_Font* gFont = TTF_OpenFont(path, size);

    if(gFont == nullptr){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
    }
}

void play(Mix_Music *gMusic){
    if(gMusic == nullptr) return;

    if(Mix_PlayingMusic() == 0){
        Mix_PlayMusic(gMusic, -1);
    }

    else if(Mix_PausedMusic() == 1){
        Mix_ResumeMusic();
    }
}

Mix_Chunk* loadSound(const char* path) {
    Mix_Chunk* gChunk = Mix_LoadWAV(path);
    if (gChunk == nullptr) {
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR,
                   "Could not load sound! SDL_mixer Error: %s", Mix_GetError());
    }
}

Mix_Music *loadMusic(const char* path){
    Mix_Music *gMusic = Mix_LoadMUS(path);
    return gMusic;
}

void play(Mix_Chunk* gChunk) {
    if(gChunk != nullptr) {
        Mix_PlayChannel(-1, gChunk, 0);
    }
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen){
    ifstream inp("best.txt");

    inp >> best;

    inp.close();

    int flag = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;

    if(!SDL_Init(SDL_INIT_EVERYTHING)){
        window = SDL_CreateWindow(title, xpos, ypos, width, height, flag);

        renderer = SDL_CreateRenderer(window, -1, 0);

        isRunning = true;
    } else {
        isRunning = false;
    }

    TTF_Init();

    for(int i = 0; i < 11; i++){
        char const *d = ("walking/w" + to_string(i + 1) + ".png").c_str();
        walk[i][0] = loadTexture(d, renderer);
    }

    for(int i = 0; i < 11; i++){
        char const *c = ("walking/walking" + to_string(i + 1) + ".png").c_str();
        walk[i][1] = loadTexture(c, renderer);
    }

    for(int i = 0; i < 13; i++){
        char const *c = ("animation/thunder" + to_string(i + 1) + ".png").c_str();
        skill[i] = loadTexture(c, renderer);
    }

    for(int i = 0; i < 13; i++){
        char const *c  = ("animation/book" + to_string(i + 1) + ".png").c_str();
        b[i] = loadTexture(c, renderer);
    }

    for(int i = 0; i < 13; i++){
        char const *d  = ("animation/b" + to_string(i + 1) + ".png").c_str();
        book[i] = loadTexture(d, renderer);
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    background = loadTexture("images/background.png", renderer);
    resume = loadTexture("images/resume.png", renderer);
    st = loadTexture("images/begin.png", renderer);
    sword = loadTexture("images/sword.png", renderer);
    shield = loadTexture("images/shield.png", renderer);
    ghost1 = loadTexture("ghost/ghost1.png", renderer);
    ghost2 = loadTexture("ghost/ghost2.png", renderer);
    angry1 = loadTexture("ghost/angry1.png", renderer);
    angry2 = loadTexture("ghost/angry2.png", renderer);
    blood = loadTexture("images/blood.jpg", renderer);
    sword2 = loadTexture("images/sword2.png", renderer);
    dash_particle = loadTexture("images/dash_particle.png", renderer);
    dash = loadTexture("images/dash.jpg", renderer);
    cd = loadTexture("images/cd.png", renderer);
    windwall = loadTexture("images/windwall.jpg", renderer);
    Chest = loadTexture("images/chest.png", renderer);
    harm = loadTexture("images/harm.png", renderer);
    HP = loadTexture("images/HP.png", renderer);
    shielded = loadTexture("images/shielded.png", renderer);
    heart = loadTexture("images/heart.png", renderer);
    speed = loadTexture("images/speed.png", renderer);
    speed2 = loadTexture("images/speed2.png", renderer);
    heart2 = loadTexture("images/heart2.png", renderer);
    font = loadFont("PakPresiden-zrm53.ttf", 100);
    TTF_Font *font50 = loadFont("PakPresiden-zrm53.ttf", 50);
    takescore = renderText("+100", font50, {236, 221, 34, 255}, renderer);
    killghost = renderText("+20", font50, {236, 221, 34, 255}, renderer);
    killangry = renderText("+50", font50, {236, 221, 34, 255}, renderer);

    harmSound = loadSound("music/harm.mp3");
    gameOver = loadSound("music/over.mp3");
    dashSound = loadSound("music/dash.mp3");
    skillSound = loadSound("music/skill.mp3");
    windwallSound = loadSound("music/windwall.mp3");
    speedboost = loadSound("music/speedboost.mp3");
    block = loadSound("music/block.mp3");
    jump = loadSound("music/jump.mp3");
    takescoreSound = loadSound("music/takescoreSound.mp3");
    music = loadMusic("music/music.mp3");
    CD = loadSound("music/cd.mp3");
    heal = loadSound("music/heal.mp3");
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

double event = 2;
int cnt, t, hp = 3;
double xLast, yLast;
double base = 160;
double g = 260; double xV;
double yV = base, xGhost;
double ghostV = 55;
bool isGhost, goLeft;
int FPS = 60, xChest;
bool isLeft = 1;
bool angry, cast;
double dSpeed;
double dShield;
bool chest;
int book_t;

struct object{
    double x, y = land, yV = 0; int t;

    void upd(){
        if(!t && y != land){
            yV -= g / FPS;
            y = min(land + 20, y - yV / FPS);
        }

        if(t) x -= yV / FPS;
    }
};

struct particle{
    double x, y, d;
    SDL_Texture* image;
};

vector<object> q, tmp;
vector<particle> p, tmp2;

mt19937 rd(time(NULL));

SDL_Rect dash_cd_rect, ghosthb,
         wall, chest_rect, hb, skillhb,
         windwall_cd_rect, skill_cd_rect;

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

void Game::update(){
    if(dSpeed > 0) xV = 140, dSpeed -= 1.0 / FPS; else xV = 70;

    event -= 1.0 / FPS;

    if(cast) return(void(t = 0));

    const Uint8* key = SDL_GetKeyboardState(NULL);

    if(yPlayer - land){
        yPlayer = min(land, yPlayer - yV / FPS);
        yV -= g / FPS;
    }

    if(key[SDL_SCANCODE_RIGHT] || key[SDL_SCANCODE_D]){
        xPlayer += xV / FPS;
        xPlayer = min(xPlayer, 750.0);

        if(isLeft) t = 0;
        isLeft = 0;
    } else

    if(key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_A]){
        xPlayer -= xV / FPS;
        xPlayer = max(0.0, xPlayer);

        if(!isLeft) t = 0;
        isLeft = 1;
    } else t = 0;

    if(yPlayer == land && (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP])){
        play(jump);

        yPlayer = min(land, yPlayer - yV / FPS);
        yV = base - g / FPS;
    }

    if(event <= 0){
        if(rd() % 17 == 0) q.push_back({750, land + 50, rd() % 120 + 60, 1});

        q.push_back({(rd() + rd() + rd())% 700 + 10, 1, 0, 0});

        event = 0.4;
    }

    if(dash_cd_rect.w <= 0 && key[SDL_SCANCODE_L]){
        dash_cd_rect.x = 30;
        dash_cd_rect.y = 520;
        dash_cd_rect.h = 56;
        dash_cd_rect.w = 65;

        xLast = xPlayer;
        yLast = yPlayer;

        p.push_back({xLast, yLast, 0.15, dash_particle});
        play(dashSound);

        if(isLeft){
            xPlayer -= xV;
            xPlayer = max(0.0, xPlayer);
        } else {
            xPlayer += xV;
            xPlayer = min(700.0, xPlayer);
        }
    }

    if(yPlayer == land && skill_cd_rect.w <= 0 && key[SDL_SCANCODE_J]){
        skill_cd_rect.x = 190;
        skill_cd_rect.y = 520;
        skill_cd_rect.w = 65;
        skill_cd_rect.h = 56;

        play(skillSound);

        cast = 1;
    }

    if(windwall_cd_rect.w <= 0 && key[SDL_SCANCODE_K]){
        play(windwallSound);

        windwall_cd_rect.x = 110;
        windwall_cd_rect.y = 520;
        windwall_cd_rect.h = 56;
        windwall_cd_rect.w = 65;

        wall.h = 100;
        wall.w = 20;

        if(isLeft){
            wall.x = xPlayer - 70;
            wall.y = yPlayer;
        } else {
            wall.x = xPlayer + 70;
            wall.y = yPlayer;
        }
    }
}

bool intersect(SDL_Rect a, SDL_Rect b){
    if(a.x + a.w < b.x) return 0;
    if(a.x > b.x + b.w) return 0;
    if(a.y + a.h < b.y) return 0;
    if(a.y > b.y + b.h) return 0;

    return 1;
}

void Game::render(){
    if(book_t == 14) cast = book_t = 0;
    if(cast && cnt % 3 == 0) book_t++;

    if(cast){
        skillhb.x = xPlayer + 90 - 105 * (isLeft);
        skillhb.y = yPlayer;
        skillhb.h = 20 + book_t * 5;
        skillhb.w = 20;
    }

    if(isGhost && cast && intersect(skillhb, ghosthb)){
        if(angry){
            ghostV /= 3; isGhost = 0; score += 50; angry = 0;
            play(takescoreSound);

            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killangry});
        } else {
            int t1 = xGhost;

            xGhost = rd() % 675 + 75;
            ghosthb.x = xGhost + 4;

            if(abs(xGhost - xPlayer) <= 250){
                isGhost = 0; score += 20;
                play(takescoreSound);

                p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killghost});
            } else {
                angry = 1;
                ghostV *= 3;

                p.push_back({t1, land, 0.15, dash_particle});
                play(dashSound);

                if(xGhost > xPlayer) goLeft = 0;
                else goLeft = 1;
            }
        }
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    hb.x = xPlayer + 29 - (dShield > 0) * 10;
    hb.y = yPlayer + 29 - (dShield > 0) * 8;

    ghosthb.x = xGhost + 4;
    ghosthb.y = land + 50;
    ghosthb.w = 22;
    ghosthb.h = 34;

    if(isGhost && chest && intersect(ghosthb, chest_rect)) chest = 0;
    if(isGhost && intersect(ghosthb, hb)){
        if(dShield <= 0){
            p.push_back({xPlayer + 25, yPlayer - 20, 0.2, harm});
            hp--; play(harmSound);
        } else {
            p.push_back({xPlayer + 25, yPlayer - 20, 0.2, shielded});
            play(block);
        }
        goLeft ^= 1;

        if(xGhost + 20 >= 750) xGhost -= 14; else xGhost += 14;
    }

    hb.h = 58 + (dShield > 0) * 20;
    hb.w = 33 + (dShield > 0) * 20;

    cnt++;
    cnt %= FPS;

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
                sw.x = u.x+9;
                sw.y = u.y;
                sw.h = 77;
                sw.w = 28;
            } else {
                sw.x = u.x + 6;
                sw.y = u.y + 15;
                sw.h = 20;
                sw.w = 49;
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

    renderTexture(dash, 30, 520, renderer);
    renderTexture(windwall, 110, 520, renderer);
    renderTexture(blood, 190, 520, renderer);
    renderTexture(heart, 500, 500, renderer);

    if(!chest && rd() % 500 == 0){
        xChest = rd() % 650 + 75;

        chest_rect.y = land + 55;
        chest_rect.x = xChest;
        chest_rect.h = 32;
        chest_rect.w = 42;

        while(abs(xChest - xPlayer) <= 30){
            xChest = rd() % 650 + 75;
            chest_rect.x = xChest;
        }

        chest = 1;
    }

    if(!isGhost){
        xGhost = rd() % 675 + 75;
        ghosthb.x = xGhost + 4;

        if(rd() % 170 == 0 && abs(xGhost - xPlayer) >= 100) isGhost = 1;

        if(xGhost > xPlayer) goLeft = 0;
        else goLeft = 1;
    } else {
        if(isLeft && !goLeft && xPlayer > xGhost && xPlayer - xGhost <= 100 && cast) goLeft = 1;
        if(!isLeft && goLeft && xPlayer < xGhost && xGhost - xPlayer <= 100 && cast) goLeft = 0;

        if(xGhost == 0) goLeft = 0;
        if(xGhost == 750) goLeft = 1;

        if(!(wall.w > 0 && intersect(ghosthb, wall)))

        if(goLeft){
            xGhost -= ghostV / FPS;
            xGhost = max(0.0, xGhost);
        } else {
            xGhost += ghostV / FPS;
            xGhost = min(xGhost, 750.0);
        }


        if(cnt % 60 < 30){
            if(angry) renderTexture(angry1, xGhost, land, renderer);
                 else renderTexture(ghost1, xGhost, land, renderer);
        } else {
            if(angry) renderTexture(angry2, xGhost, land, renderer);
                 else renderTexture(ghost2, xGhost, land, renderer);
        }
    }

    if(chest && intersect(chest_rect, hb)){
        int chestType = rd() % (4 + (hp != 3));

        if(chestType == 0){
            score += 100;
            play(takescoreSound);
            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, takescore});
        }

        if(chestType == 4){
            p.push_back({xPlayer, yPlayer, 0.23, HP});
            play(heal); hp = 3;
        }

        if(chestType == 3){
            play(speedboost);
            dSpeed = 5;
        }

        if(chestType == 1) dShield = 4;

        if(chestType == 2){
            play(CD);
            dash_cd_rect.w = 0;
            skill_cd_rect.w = 0;
            windwall_cd_rect.w = 0;
            p.push_back({xPlayer + 25, yPlayer - 20, 0.20, cd});
        }

        chest = 0;
    }

    if(dShield > 0){
        renderTexture(shield, xPlayer, yPlayer, renderer);
        dShield -= 1.0 / FPS;
    }

    renderTexture(walk[t % 11][isLeft], xPlayer, yPlayer, renderer);

    if(isLeft)renderTexture(book[book_t % 13], xPlayer, yPlayer, renderer);
         else renderTexture(b[book_t % 13], xPlayer, yPlayer, renderer);

    if(book_t){
        if(isLeft) renderTexture(skill[(book_t - 1) % 13], xPlayer - 97, yPlayer, renderer);
              else renderTexture(skill[(book_t - 1) % 13], xPlayer + 5, yPlayer, renderer);
    }

    if(dSpeed > 0){
        if(t % 8 < 4) renderTexture(speed, xPlayer, yPlayer, renderer);
        else renderTexture(speed2, xPlayer, yPlayer, renderer);
    }


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
                    Mix_ResumeMusic();
                    xPlayer = 0;
                    yPlayer = land;
                    isLeft = 1;
                    isGhost = 0;
                    cast = 0;
                    event = 2;
                    ghostV = 55;
                    angry = 0;
                    skill_cd_rect.w = 0;
                    book_t = 0;
                    wall.w = 0;
                    xLast = 0;
                    dSpeed = 0;
                    yLast = 0;
                    base = 160;
                    yV = base;
                    cnt = 0;
                    t = 0;
                    hp = 3;
                    dash_cd_rect.w = 0;
                    windwall_cd_rect.w = 0;
                    chest = 0;
                    score = 0;

                    p.clear();
                    q.clear();
                    tmp.clear();
                    tmp2.clear();

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

    if(hp > 1) renderTexture(heart, 600, 500, renderer);
          else renderTexture(heart2, 600, 500, renderer);
    if(hp > 2) renderTexture(heart, 700, 500, renderer);
          else renderTexture(heart2, 700, 500, renderer);

    if(cnt % 8 == 0){
        dash_cd_rect.x += 1.0;
        dash_cd_rect.w -= 1.0;
    }

    if(cnt % 5 == 0){
        skill_cd_rect.x += 1.0;
        skill_cd_rect.w -= 1.0;
    }

    if(cnt % 32 == 0){
        windwall_cd_rect.x += 1.0;
        windwall_cd_rect.w -= 1.0;
    }

    if(dash_cd_rect.w > 0) SDL_RenderFillRect(renderer, &dash_cd_rect);
    if(skill_cd_rect.w > 0) SDL_RenderFillRect(renderer, &skill_cd_rect);
    if(windwall_cd_rect.w > 0) SDL_RenderFillRect(renderer, &windwall_cd_rect);

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
