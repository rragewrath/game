double xPlayer = 0;
double yPlayer = land;
double event = 2;
int cnt, t, hp = 3;
double xLast, yLast;
double base = 160;
double g = 260; double xV;
double yV = base, xGhost;
double ghostV = 60;
bool isGhost, goLeft;
int FPS = 60, xChest, score;
bool isLeft = 1;
bool angry, cast;
double dSpeed;
double dShield;
int book_t;
bool chest;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *background;
SDL_Texture *resume;
SDL_Texture *st;
Mix_Chunk *harmSound, *gameOver;
Mix_Chunk *dashSound, *skillSound;
Mix_Chunk *takescoreSound, *CD;
Mix_Chunk *windwallSound, *heal;
Mix_Chunk *speedboost;
Mix_Chunk *jump, *block;
Mix_Music *music;
SDL_Texture *walk[11][2], *book[13];
SDL_Texture *b[13], *skill[13];
SDL_Texture *sword, *sword2;
SDL_Texture *cd, *angry1;
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
TTF_Font *font, *font50;

SDL_Rect dash_cd_rect, ghosthb,
         wall, chest_rect, hb, skillhb,
         windwall_cd_rect, skill_cd_rect;


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
    Mix_ResumeMusic();
    xPlayer = 0;
    yPlayer = land;
    isLeft = 1;
    isGhost = 0;
    cast = 0;
    event = 2;
    ghostV = 60;
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
}

bool intersect(SDL_Rect a, SDL_Rect b){
    if(a.x + a.w < b.x) return 0;
    if(a.x > b.x + b.w) return 0;
    if(a.y + a.h < b.y) return 0;
    if(a.y > b.y + b.h) return 0;

    return 1;
}
