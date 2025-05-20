void draw_triangle(double x1, double y1, double x2, double y2, double x3, double y3){
    SDL_Vertex verts[3] = {
        {.position = {x1, y1}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 }},
        {.position = {x2, y2}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 }},
        {.position = {x3, y3}, .color = { 29, 28, 65, 255 }, .tex_coord = { 0, 0 }}
    };
    SDL_RenderGeometry(renderer, NULL, verts, 3, NULL, 0);
}

void pcd(SDL_Rect a, double &t, double cd){
    if(dSpeed > 0) cd *= 2; if(t == -1) return;
    int x1 = a.x, y1 = a.y; int h = a.h, w = a.w;
    int x2 = x1 + w, y2 = y1 + h; int p = 2 * (h + w);

    if(t <= p / 8) draw_triangle(x1 + w / 2.0, y1 + h / 2.0, x2, y1, x1 + w/2.0 + (t*w*4.0)/p, y1);
    if(t <= (3 * p) / 8) draw_triangle(x1 + w / 2.0, y1 + h / 2.0,x2, y2,x2, y1 + (max(0.0, t - p/8)*h*4.0)/p);
    if(t <= (5*p) / 8) draw_triangle(x1 + w / 2.0, y1 + h / 2.0, x1, y2, x2 - (max(0.0, t - (3 * p)/8)*w*4.0)/p, y2);
    if(t <= (7*p) / 8) draw_triangle(x1 + w / 2.0, y1 + h / 2.0, x1, y1, x1, y2 - (max(0.0, t - (5*p)/8)*h*4.0)/p);
    if(t <= p) draw_triangle(x1 + w / 2.0, y1 + h / 2.0, x1 + w / 2.0, y1, x1 + (max(0.0, t - (7*p)/8)*w*4.0)/p, y1);

    t += 3 * cd / gameSpeed;
    if(t > p) t = -1;
}


void cdr(){
    pcd(dash_cd_rect, dash_cnt, 11);
    pcd(skill_cd_rect, skill_cnt, 12 - skill_cd_mode * 7);
    pcd(windwall_cd_rect, windwall_cnt, 4);
}

void loadAll(SDL_Renderer* renderer, int volume){
    Mix_VolumeMusic(volume); Mix_Volume(-1, volume); SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); stone = loadTexture("images/stone.png", renderer);
    for(int i = 0; i < 11; i++){char const *d = ("walking/w" + to_string(i + 1) + ".png").c_str(); walk[i][0] = loadTexture(d, renderer);}
    for(int i = 0; i < 11; i++){char const *c = ("walking/walking" + to_string(i + 1) + ".png").c_str(); walk[i][1] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c = ("animation/thunder" + to_string(i + 1) + ".png").c_str(); skill[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c  = ("animation/book" + to_string(i + 1) + ".png").c_str(); b[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *d  = ("animation/b" + to_string(i + 1) + ".png").c_str(); book[i] = loadTexture(d, renderer);}
    for(int i = 0; i < 16; i++){char const *d = ("animation/healing" + to_string(i + 1) + ".png").c_str(); heal_particle[i] = loadTexture(d, renderer);}
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); menu_sound = loadSound("sound/menuselection.mp3"); ending = loadTexture("images/over.jpg", renderer);
    background = loadTexture("images/background.png", renderer); pause = loadTexture("images/pause.png", renderer); lava = loadTexture("images/lava.png", renderer);
    sword = loadTexture("images/sword.png", renderer); sword2 = loadTexture("images/sword2.png", renderer);
    shield = loadTexture("images/shield.png", renderer); menu = loadTexture("images/menu.png", renderer);
    ghost1 = loadTexture("ghost/ghost1.png", renderer); ghost2 = loadTexture("ghost/ghost2.png", renderer);
    angry1 = loadTexture("ghost/angry1.png", renderer); angry2 = loadTexture("ghost/angry2.png", renderer);
    thunder = loadTexture("images/thunder.jpg", renderer); settings = loadTexture("images/settings.png", renderer);
    dash_particle[0] = loadTexture("images/dash_particle.png", renderer); dash = loadTexture("images/dash.jpg", renderer);
    dash_particle[1] = loadTexture("images/dash_particle2.png", renderer); windwall = loadTexture("images/windwall.jpg", renderer);
    harm = loadTexture("images/harm.png", renderer); shielded = loadTexture("images/shielded.png", renderer);
    Chest = loadTexture("images/chest.png", renderer); superjump = loadTexture("images/superjump.png", renderer);
    heart2 = loadTexture("images/heart2.png", renderer); heart = loadTexture("images/heart.png", renderer);
    font = loadFont("ROGLyonsTypeRegular3.ttf", 80); font50 = loadFont("ROGLyonsTypeRegular3.ttf", 33);
    killghost = renderText("+20", font50, {236, 221, 34, 255}, renderer); p5 = renderText("+5", font50, {236, 221, 34, 255}, renderer);
    killangry = renderText("+50", font50, {236, 221, 34, 255}, renderer); speedup = loadSound("sound/speedup.mp3");
    harmSound = loadSound("sound/harm.mp3"); windwallSound = loadSound("sound/windwall.mp3");
    gameOver = loadSound("sound/over.mp3"); dashSound = loadSound("sound/dash.mp3");
    skillSound = loadSound("sound/skill.mp3"); menu_rect.h = 103;
    windwall_image = loadTexture("images/windwall_image.png", renderer);
    block = loadSound("sound/block.mp3"); jump = loadSound("sound/jump.mp3");
    takescoreSound = loadSound("sound/takescoreSound.mp3");
    music = loadMusic("sound/music.mp3"); heal = loadSound("sound/heal.mp3");
    resume_rect.x = 230; resume_rect.y = 228; resume_rect.w = 340; resume_rect.h = 103;
    menu_rect.x = 230; menu_rect.y = 368; menu_rect.w = 340; menu_rect.h = 103;
    hb.push_back({36, 30, 20, 20}); base_hb.push_back({36, 30, 20, 20});
    hb.push_back({31, 50, 30, 20}); base_hb.push_back({31, 50, 30, 20});
    hb.push_back({36, 65, 20, 20}); base_hb.push_back({36, 65, 20, 20});
    skill_cd_rect.x = 165+1 - 20; skill_cd_rect.y = 530+4;
    skill_cd_rect.w = 65-2; skill_cd_rect.h = 56-1;
    windwall_cd_rect.x = 85+1 - 10; windwall_cd_rect.y = 530+4;
    windwall_cd_rect.h = 56-1; windwall_cd_rect.w = 65-2;
    dash_cd_rect.x = 5+1; dash_cd_rect.y = 530+4;
    dash_cd_rect.h = 56-1; dash_cd_rect.w = 65-2;
    lava_rect.h = 10; lava_rect.w = 800; isRise = 1; lava_rect.x = 0;
    SDL_SetTextureAlphaMod(ghost1, 200);
    SDL_SetTextureAlphaMod(ghost2, 200);
}

void draw_effect(){
    char const *p1 = ("Score: " + to_string(score)).c_str();
    SDL_Texture *SCORE = renderText(p1, font50, {236, 221, 34, 255}, renderer);

    renderTexture(SCORE, 242, 548, renderer);

    if(dShield > 0){
        renderTexture(shield, xPlayer, yPlayer, renderer);
        dShield -= 1.0 / gameSpeed;
    }

    if(dSpeed > 0){
        int B = max(0, (int)movement.size() - 21), alphamod = 5;

        for(int i = B; i < movement.size(); i += 5){
            SDL_SetTextureAlphaMod(movement[i].image, alphamod); alphamod += 20;
            renderTexture(movement[i].image, movement[i].x, movement[i].y, renderer);
            SDL_SetTextureAlphaMod(movement[i].image, 255);
        }

        movement.push_back({xPlayer, yPlayer, 0, walk[t % 11][isLeft]});
    } else movement.clear();

    renderTexture(walk[t % 11][isLeft], xPlayer, yPlayer, renderer);

    if(healing){
        if(healing >= 16) healing = 0; else healing++;
        if(healing) renderTexture(heal_particle[healing - 1], xPlayer - 14, yPlayer - 9, renderer);
    }

    if(isLeft)renderTexture(book[book_t % 13], xPlayer, yPlayer - 6, renderer);
         else renderTexture(b[book_t % 13], xPlayer - 3, yPlayer - 6, renderer);

    if(book_t){
        for(int k = 0; k < skillhb.w; k += 30) renderTexture(skill[(book_t - 1) % 13], skillhb.x - 75 + k, skillhb.y - 17, renderer);
    }

    renderTexture(dash, 5, 533, renderer);
    renderTexture(windwall, 85 - 10, 533, renderer);
    renderTexture(thunder, 165 - 20, 533, renderer);
    renderTexture(heart, 500, 515, renderer);

    if(hp > 1) renderTexture(heart, 600, 515, renderer);
          else renderTexture(heart2, 600, 515, renderer);
    if(hp > 2) renderTexture(heart, 700, 515, renderer);
          else renderTexture(heart2, 700, 515, renderer);

    while(p.size()){
        auto u = p.back();
        p.pop_back();

        SDL_SetTextureAlphaMod(u.image, min(255, int(u.d*4000)));

        if(u.d > 0){
            renderTexture(u.image, u.x, u.y, renderer);
            u.d -= 0.45 / gameSpeed;
            tmp2.push_back(u);
        }
    }

    swap(tmp2, p);

    if(wall.w > 0){
        if(cnt % 16 == 1) wall.w -= 2, wall.x++;
        ww.pop_back(); ww.push_back(wall);
        SDL_RenderCopy(renderer, windwall_image, NULL, &wall);
    } else if(ww.size()) ww.pop_back();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    if(chest) renderTexture(Chest, xChest, land, renderer);
    cdr();
}

void game_over(int &best, bool &isRunning, bool &isMenu){
    SDL_Rect rect = {0, 0, 800, 600}; double t = 0;

    SDL_Event events;
    Mix_HaltChannel(-1); Mix_PauseMusic(); play(gameOver);

    int tW, tH;

    while(t != -1){
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, ending, NULL, NULL);
        int temp = gameSpeed; gameSpeed = 58;
        pcd(rect, t, 50); gameSpeed = temp;
        SDL_RenderPresent(renderer);
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, ending, NULL, NULL);

    best = max(best, score);

    char const *p1 = ("Score: " + to_string(score)).c_str();
    SDL_Texture* SCORE = renderText(p1, font, {236, 221, 34, 255}, renderer);
    SDL_QueryTexture(SCORE, NULL, NULL, &tW, &tH);
    SDL_Rect textRect = {(800 - tW) / 2, 420, tW, tH};
    SDL_RenderCopy(renderer, SCORE, NULL, &textRect);

    SDL_RenderPresent(renderer);
    SDL_Delay(170);

    char const *p2 = ("Best: " + to_string(best)).c_str();
    SDL_Texture* HIGH = renderText(p2, font, {235, 51, 36, 255}, renderer);
    SDL_QueryTexture(HIGH, NULL, NULL, &tW, &tH);
    SDL_Rect textRECT = {(800 - tW) / 2, 515, tW, tH};
    SDL_RenderCopy(renderer, HIGH, NULL, &textRECT);

    SDL_Rect rep = {178, 315, 196, 74};
    SDL_Rect MENU = {412, 315, 196, 74};
    SDL_RenderPresent(renderer);

    SDL_DestroyTexture(SCORE); SCORE = NULL;
    SDL_DestroyTexture(HIGH); HIGH = NULL;

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
