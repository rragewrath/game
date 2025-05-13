double xPlayer = 0;
double yPlayer = land;
double event = 2;
long long cnt, t;
double dash_cnt = -1;
double skill_cnt = -1;
double windwall_cnt= -1;
int hp = 3, ticks = 0, healing = 0, alpha, diff;
double xLast, yLast, base = 160;
double g = 260; double xV;
double yV = base, xGhost, ghostV = 60;
bool isGhost, goLeft;
int xChest, score, gameSpeed = 68;
bool isLeft = 1, ghost_resit;
bool angry;
double dSpeed, dShield;
int book_t;
bool chest, skill_cd_mode;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *background;
SDL_Texture *pause, *menu;
Mix_Chunk *harmSound, *gameOver;
Mix_Chunk *dashSound, *skillSound;
Mix_Chunk *takescoreSound, *CD;
Mix_Chunk *windwallSound, *heal;
Mix_Chunk *speedboost;
Mix_Chunk *jump, *block, *menu_sound;
Mix_Music *music;
SDL_Texture *walk[11][2], *book[13];
SDL_Texture *b[13], *skill[13];
SDL_Texture *sword, *sword2;
SDL_Texture *cd, *angry1, *superjump;
SDL_Texture *angry2, *settings;
SDL_Texture *thunder;
SDL_Texture *ghost1;
SDL_Texture *ghost2;
SDL_Texture *dash_particle[2];
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
TTF_Font *font, *font50;

vector<SDL_Rect> hb, base_hb;

SDL_Rect dash_cd_rect, ghosthb, menu_rect, resume_rect,
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
vector<particle> p, tmp2;

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

void reset(){
    Mix_HaltChannel(-1);
    Mix_RewindMusic();
    xPlayer = 0; yPlayer = land;
    isLeft = 1; event = 2;
    isGhost = wall.w = 0;
    ghost_resit = ticks = 0;
    ghostV = 60;
    angry = dShield = 0;
    dash_cnt = skill_cnt = windwall_cnt = -1;
    xLast = dSpeed = 0;
    yLast = healing = 0;
    base = 160; yV = base;
    cnt = t = 0; hp = 3;
    chest = score = 0;
    p.clear(); q.clear();
    tmp.clear(); tmp2.clear();
}

bool intersect(SDL_Rect a, SDL_Rect b){
    if(a.x + a.w < b.x) return 0;
    if(a.x > b.x + b.w) return 0;
    if(a.y + a.h < b.y) return 0;
    if(a.y > b.y + b.h) return 0;

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
