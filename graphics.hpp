void pcd(SDL_Rect a, double &t, double cd){
    if(t == -1) return;
    int x1 = a.x, y1 = a.y; int h = a.h, w = a.w;
    int x2 = x1 + w, y2 = y1 + h; int p = 2 * (h + w);

    if(t <= p / 8){
        SDL_Vertex verts[3] = {
            { .position = { x1 + w / 2.0, y1 + h / 2.0}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x2, y1},  .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1 + w/2.0 + (t*w*4.0)/p, y1}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } }
        };
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }

    if(t <= (3 * p) / 8){
        SDL_Vertex verts[3] = {
            { .position = { x1 + w / 2.0, y1 + h / 2.0}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x2, y2},  .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x2, y1 + (max(0.0, t - p/8)*h*4.0)/p}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } }
        };
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }

    if(t <= (5*p) / 8){
        SDL_Vertex verts[3] = {
            { .position = { x1 + w / 2.0, y1 + h / 2.0}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1, y2},  .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x2 - (max(0.0, t - (3 * p)/8)*w*4.0)/p, y2}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } }
        };
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }

    if(t <= (7*p) / 8){
        SDL_Vertex verts[3] = {
            { .position = { x1 + w / 2.0, y1 + h / 2.0}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1, y1},  .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1, y2 - (max(0.0, t - (5*p)/8)*h*4.0)/p}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } }
        };
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }
    if(t <= p){
        SDL_Vertex verts[3] = {
            { .position = { x1 + w / 2.0, y1 + h / 2.0}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1 + w / 2.0, y1},  .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } },
            { .position = { x1 + (max(0.0, t - (7*p)/8)*w*4.0)/p, y1}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 } }
        };
        SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
    }

    t += 3 * cd / gameSpeed;
    if(t > p) t = -1;
}


void cdr(){
    pcd(dash_cd_rect, dash_cnt, 13);
    pcd(skill_cd_rect, skill_cnt, 13 - skill_cd_mode * 7);
    pcd(windwall_cd_rect, windwall_cnt, 4);
}

void loadAll(SDL_Renderer* renderer, int volume){
    Mix_VolumeMusic(volume); Mix_Volume(-1, volume); SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for(int i = 0; i < 11; i++){char const *d = ("walking/w" + to_string(i + 1) + ".png").c_str(); walk[i][0] = loadTexture(d, renderer);}
    for(int i = 0; i < 11; i++){char const *c = ("walking/walking" + to_string(i + 1) + ".png").c_str(); walk[i][1] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c = ("animation/thunder" + to_string(i + 1) + ".png").c_str(); skill[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c  = ("animation/book" + to_string(i + 1) + ".png").c_str(); b[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *d  = ("animation/b" + to_string(i + 1) + ".png").c_str(); book[i] = loadTexture(d, renderer);}
    for(int i = 0; i < 16; i++){char const *d = ("animation/healing" + to_string(i + 1) + ".png").c_str(); heal_particle[i] = loadTexture(d, renderer);}
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); menu_sound = loadSound("music/menuselection.mp3");
    background = loadTexture("images/background.png", renderer); pause = loadTexture("images/pause.png", renderer);
    sword = loadTexture("images/sword.png", renderer); sword2 = loadTexture("images/sword2.png", renderer);
    shield = loadTexture("images/shield.png", renderer); menu = loadTexture("images/menu.png", renderer);
    ghost1 = loadTexture("ghost/ghost1.png", renderer); ghost2 = loadTexture("ghost/ghost2.png", renderer);
    angry1 = loadTexture("ghost/angry1.png", renderer); angry2 = loadTexture("ghost/angry2.png", renderer);
    thunder = loadTexture("images/thunder.jpg", renderer); settings = loadTexture("images/settings.png", renderer);
    dash_particle[0] = loadTexture("images/dash_particle.png", renderer); dash = loadTexture("images/dash.jpg", renderer);
    dash_particle[1] = loadTexture("images/dash_particle2.png", renderer);
    cd = loadTexture("images/cd.png", renderer); windwall = loadTexture("images/windwall.jpg", renderer);
    Chest = loadTexture("images/chest.png", renderer); superjump = loadTexture("images/superjump.png", renderer);
    harm = loadTexture("images/harm.png", renderer); shielded = loadTexture("images/shielded.png", renderer);
    speed = loadTexture("images/speed.png", renderer); speed2 = loadTexture("images/speed2.png", renderer);
    heart2 = loadTexture("images/heart2.png", renderer); heart = loadTexture("images/heart.png", renderer);
    font = loadFont("ROGLyonsTypeRegular3.ttf", 80); font50 = loadFont("ROGLyonsTypeRegular3.ttf", 33);
    takescore = renderText("+100", font50, {236, 221, 34, 255}, renderer);
    killghost = renderText("+20", font50, {236, 221, 34, 255}, renderer);
    killangry = renderText("+50", font50, {236, 221, 34, 255}, renderer);
    harmSound = loadSound("music/harm.mp3"); windwallSound = loadSound("music/windwall.mp3");
    gameOver = loadSound("music/over.mp3"); dashSound = loadSound("music/dash.mp3");
    skillSound = loadSound("music/skill.mp3"); speedboost = loadSound("music/speedboost.mp3");
    block = loadSound("music/block.mp3"); jump = loadSound("music/jump.mp3");
    takescoreSound = loadSound("music/takescoreSound.mp3");
    music = loadMusic("music/music.mp3"); CD = loadSound("music/cd.mp3"); heal = loadSound("music/heal.mp3");
    resume_rect.x = 230; resume_rect.y = 228; resume_rect.w = 340; resume_rect.h = 103;
    menu_rect.x = 230; menu_rect.y = 368; menu_rect.w = 340; menu_rect.h = 103;
    hb.push_back({36, 30, 20, 20}); base_hb.push_back({36, 30, 20, 20});
    hb.push_back({31, 50, 30, 20}); base_hb.push_back({31, 50, 30, 20});
    hb.push_back({36, 65, 20, 20}); base_hb.push_back({36, 65, 20, 20});
    skill_cd_rect.x = 165+1; skill_cd_rect.y = 530+1;
    skill_cd_rect.w = 65-1; skill_cd_rect.h = 56-1;
    windwall_cd_rect.x = 85+1; windwall_cd_rect.y = 530+1;
    windwall_cd_rect.h = 56-1; windwall_cd_rect.w = 65-1;
    dash_cd_rect.x = 5+1; dash_cd_rect.y = 530+1;
    dash_cd_rect.h = 56-1; dash_cd_rect.w = 65-1;
}

void draw_effect(){
    char const *p1 = ("Score: " + to_string(score)).c_str();
    SDL_Texture *SCORE = renderText(p1, font50, {236, 221, 34, 255}, renderer);

    renderTexture(SCORE, 242, 543, renderer);

    if(dShield > 0){
        renderTexture(shield, xPlayer, yPlayer, renderer);
        dShield -= 1.0 / gameSpeed;
    }

    renderTexture(walk[t % 11][isLeft], xPlayer, yPlayer, renderer);

    if(healing){
        if(healing) renderTexture(heal_particle[healing - 1], xPlayer - 14, yPlayer - 9, renderer);
        if(healing > 16) healing = 0; else healing++;
    }

    if(isLeft)renderTexture(book[book_t % 13], xPlayer, yPlayer - 6, renderer);
         else renderTexture(b[book_t % 13], xPlayer - 3, yPlayer - 6, renderer);

    if(book_t){
        for(int k = 0; k < skillhb.w; k += 30) renderTexture(skill[(book_t - 1) % 13], skillhb.x - 75 + k, skillhb.y - 17, renderer);
    }

    if(dSpeed > 0){
        if(cnt % 12 < 6) renderTexture(speed, xPlayer, yPlayer, renderer);
        else renderTexture(speed2, xPlayer, yPlayer, renderer);
    }

    renderTexture(dash, 5, 530, renderer);
    renderTexture(windwall, 85, 530, renderer);
    renderTexture(thunder, 165, 530, renderer);
    renderTexture(heart, 500, 500, renderer);

    if(hp > 1) renderTexture(heart, 600, 500, renderer);
          else renderTexture(heart2, 600, 500, renderer);
    if(hp > 2) renderTexture(heart, 700, 500, renderer);
          else renderTexture(heart2, 700, 500, renderer);

    while(p.size()){
        auto u = p.back();
        p.pop_back();

        SDL_SetTextureAlphaMod(u.image, min(255, int(u.d*5000)));

        if(u.d > 0){
            renderTexture(u.image, u.x, u.y, renderer);
            u.d -= 0.33 / gameSpeed;
            tmp2.push_back(u);
        }
    }

    swap(tmp2, p);


    if(wall.w > 0){
        SDL_SetRenderDrawColor(renderer, 137, 214, 205, alpha);
        alpha -= diff;
        if(cnt % 16 == 1) wall.w -= 2, wall.x++;
        SDL_RenderFillRect(renderer, &wall);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if(chest) renderTexture(Chest, xChest, land, renderer);
    cdr();
}

void game_over(int &best, bool &isRunning, bool &isMenu){
    SDL_RenderClear(renderer); SDL_Event events;
    Mix_HaltChannel(-1); Mix_PauseMusic(); play(gameOver);

    int tW, tH;
    SDL_Texture* background = loadTexture("images/over.jpg", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    best = max(best, score);

    char const *p1 = ("Score: " + to_string(score)).c_str();
    SDL_Texture* SCORE = renderText(p1, font, {236, 221, 34, 255}, renderer);
    SDL_QueryTexture(SCORE, NULL, NULL, &tW, &tH);
    SDL_Rect textRect = {(800 - tW) / 2, 420, tW, tH};
    SDL_RenderCopy(renderer, SCORE, NULL, &textRect);

    char const *p2 = ("Best: " + to_string(best)).c_str();
    SDL_Texture* HIGH = renderText(p2, font, {235, 51, 36, 255}, renderer);
    SDL_QueryTexture(HIGH, NULL, NULL, &tW, &tH);
    SDL_Rect textRECT = {(800 - tW) / 2, 515, tW, tH};
    SDL_RenderCopy(renderer, HIGH, NULL, &textRECT);

    SDL_Rect rep = {178, 315, 196, 74};
    SDL_Rect MENU = {412, 315, 196, 74};
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(background); background = NULL;
    SDL_DestroyTexture(SCORE); SCORE = NULL;

    int x, y;

    while(1){
        SDL_PollEvent(&events);
        SDL_GetMouseState(&x, &y);

        if(events.type == SDL_MOUSEBUTTONDOWN){
            SDL_Rect mouse = {x, y, 0, 0};

            if(intersect(mouse, rep)){
                reset(); play(menu_sound);
                play(music); break;
            }

            if(intersect(mouse, MENU)){
                play(menu_sound);
                isMenu = 1; break;
            }
        } else if(events.type == SDL_QUIT){
            isRunning = 0;
            break;
        }
    }
}
