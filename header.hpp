double xPlayer = 0, yPlayer = land, event = 2;
long long cnt, t;
double dash_cnt = -1, skill_cnt = -1, windwall_cnt= -1;
int hp = 3, ticks = 0, healing = 0;
double xLast, yLast, base = 330, g = 270; double xV;
double yV = base, xGhost, ghostV = 60, dSpeed, dShield;
int xChest, score, gameSpeed = 68, book_t;
bool isGhost, goLeft, isLeft = 1, ghost_resit, angry;
bool chest, skill_cd_mode, onGround = 1, isRise, isDown;
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Texture *background;
SDL_Texture *pause, *menu;
Mix_Chunk *harmSound, *gameOver;
Mix_Chunk *dashSound, *skillSound;
Mix_Chunk *takescoreSound;
Mix_Chunk *windwallSound, *heal, *speedup;
Mix_Chunk *jump, *block, *menu_sound;
Mix_Music *music;
SDL_Texture *walk[11][2], *book[13];
SDL_Texture *b[13], *skill[13], *lava;
SDL_Texture *sword, *sword2, *windwall_image;
SDL_Texture *angry1, *superjump;
SDL_Texture *angry2, *settings;
SDL_Texture *thunder, *dash, *ending;
SDL_Texture *ghost1, *ghost2, *stone;
SDL_Texture *dash_particle[2];
SDL_Texture *windwall, *shielded;
SDL_Texture *killghost, *killangry;
SDL_Texture *Chest, *harm, *shield;
SDL_Texture *heart, *p5, *heart2;
TTF_Font *font, *font50;

vector<SDL_Rect> hb, base_hb, blocks, ww;

SDL_Rect dash_cd_rect, ghosthb, menu_rect, resume_rect, lava_rect,
         wall, chest_rect, skillhb, windwall_cd_rect, skill_cd_rect;

struct object{
    double x, y = land, yV = 0; int t;

    void upd(){
        if(!t){
            yV -= min(1.25, score / 400.0 + 0.6) * g / gameSpeed;
            y = min(land + 20, y - yV / gameSpeed);
        } else x -= min(1.25, score / 400.0 + 0.6) * yV / gameSpeed;
    }
};

SDL_Texture *heal_particle[16];

struct particle{
    double x, y, d;
    SDL_Texture* image;
};

vector<object> q, tmp;
vector<particle> p, tmp2, movement;

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

void renderTexture(SDL_Texture *texture, double x, double y,
                   SDL_Renderer* renderer){
	SDL_Rect dest;
	dest.x = x; dest.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dest.w, &dest.h);

	SDL_RenderCopy(renderer, texture, NULL, &dest);
}

TTF_Font* loadFont(const char* path, int size){
    TTF_Font* gFont = TTF_OpenFont(path, size);

    if(gFont == nullptr){
        SDL_LogMessage(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_ERROR, "Load font %s", TTF_GetError());
    }

    return gFont;
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
    return gChunk;
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

int random(int l, int r){
    return l + rd() % (r - l + 1);
}

int target, max_d, lava_max_h;

void reset(){
    Mix_HaltChannel(-1);
    Mix_RewindMusic();
    xPlayer = 0; yPlayer = land;
    isLeft = 1; event = 2; isDown = 0;
    isGhost = wall.w = isRise = 0;
    ghost_resit = ticks = 0;
    ghostV = 60; onGround = 1;
    angry = dShield = book_t = 0;
    dash_cnt = skill_cnt = windwall_cnt = -1;
    xLast = dSpeed = 0; target = 50;
    yLast = healing = 0;
    base = 160; yV = base;
    cnt = t = 0; hp = 3; lava_rect.h = 10;
    chest = score = 0; lava_rect.y = 510;
    p.clear(); q.clear(); movement.clear();
    tmp.clear(); tmp2.clear();
    blocks.clear(); ww.clear();
}

bool intersect(SDL_Rect a, SDL_Rect b){
    if(a.x + a.w - 1 < b.x) return 0;
    if(a.x > b.x + b.w - 1) return 0;
    if(a.y + a.h - 1 < b.y) return 0;
    if(a.y > b.y + b.h - 1) return 0;

    return 1;
}

bool intersect(SDL_Rect a, vector<SDL_Rect> &x){
    for(auto &b: x){
        if(intersect(a, b)){
            return 1;
        }
    }

    return 0;
}

void addBlocks(int k){
    float t_up = base / g;
    float t_total = 2 * t_up;
    float max_dx = xV * t_total;
    float max_dy = (base * base) / (1.25 * g);

    int x = 0, y = 400 - rd() % 30;
    max_d = INT_MAX;

    for (int i = 0; i < k;){
        SDL_Rect p = {x, y, 100, 20};
        SDL_Rect pBlockZone = {x - 5, y - 60, 105, 60};

        bool valid = true;

        if(intersect(p, hb)) valid = false;

        for(const auto& other : blocks){
            if(intersect(p, other)){
                valid = false;
                break;
            }
            SDL_Rect blockZone = {other.x - 5, other.y - 60, other.w + 5, 60};
//            SDL_RenderFillRect(renderer, &blockZone);
//            SDL_RenderDrawRect(renderer, &p);
//            SDL_RenderPresent(renderer);
//            SDL_Delay(1000);
            if(intersect(p, blockZone) || intersect(other, pBlockZone)){
                valid = false;
                break;
            }
        }

        if(valid){
            blocks.push_back(p); ++i;
            max_d = min(max_d, y);

            int dx = rand() % (int)(max_dx - 35) + 135;
            int dy = rand() % (int)(max_dy * 1.4f) - (int)(max_dy);

            (x += dx) %= 800; y += dy;

            if (y > 450) y = 400;
            if (y < 100) y = 100;
        } else {
            x += 20;
            if(x > 750) x = 0, y += 15;
        }
    }
}

void lava_rise(){
    if(isRise){
        if(intersect(lava_rect, hb) && dShield <= 0) hp = 0;

        if(isDown) if(cnt % (gameSpeed + 5) == 0) lava_rect.y += 7, lava_rect.h -= 7;
        if(!isDown) if(cnt % (gameSpeed + 5) == 0) lava_rect.y -= 7, lava_rect.h += 7;

        SDL_RenderCopy(renderer, lava, NULL, &lava_rect);

        if(isDown && lava_rect.y > 510) lava_rect.y = 510, lava_rect.h = 10;
        if(!isDown && lava_rect.y < lava_max_h) lava_rect.y = lava_max_h, lava_rect.h = 520 - lava_max_h;

        if(lava_rect.y == lava_max_h && !isDown) isDown = 1;
        if(lava_rect.y == 510 && isDown){
            isRise = isDown = 0;
            blocks.clear();
        }
    } else if(score >= target && onGround){
        isRise = 1; target += 150; addBlocks(10);
        lava_max_h = random(max_d + 20, 450);
    }
}
