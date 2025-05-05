void cdr(){
    if(cnt % (gameSpeed > 10 ? gameSpeed / 10 : 1) == 0){
        dash_cd_rect.x += 1 + 10 - min(10, gameSpeed);
        dash_cd_rect.w -= 1 + 10 - min(10, gameSpeed);
    }

    if(cnt % (gameSpeed > 10 ? gameSpeed / 10 : 1) == 0){
        skill_cd_rect.x += 1 + 10 - min(10, gameSpeed);
        skill_cd_rect.w -= 1 + 10 - min(10, gameSpeed);
    }

    if(cnt % (3 * (gameSpeed > 10 ? gameSpeed / 10 : 1)) == 0){
        windwall_cd_rect.x += 1 + 10 - min(10, gameSpeed);
        windwall_cd_rect.w -= 1 + 10 - min(10, gameSpeed);
    }
}

void loadAll(SDL_Renderer* renderer, int volume){
    Mix_VolumeMusic(volume); Mix_Volume(-1, volume);
    for(int i = 0; i < 11; i++){char const *d = ("walking/w" + to_string(i + 1) + ".png").c_str(); walk[i][0] = loadTexture(d, renderer);}
    for(int i = 0; i < 11; i++){char const *c = ("walking/walking" + to_string(i + 1) + ".png").c_str(); walk[i][1] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c = ("animation/thunder" + to_string(i + 1) + ".png").c_str(); skill[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *c  = ("animation/book" + to_string(i + 1) + ".png").c_str(); b[i] = loadTexture(c, renderer);}
    for(int i = 0; i < 13; i++){char const *d  = ("animation/b" + to_string(i + 1) + ".png").c_str(); book[i] = loadTexture(d, renderer);}
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); menu_sound = loadSound("music/menuselection.mp3");
    background = loadTexture("images/background.png", renderer); pause = loadTexture("images/pause.png", renderer);
    sword = loadTexture("images/sword.png", renderer); sword2 = loadTexture("images/sword2.png", renderer);
    shield = loadTexture("images/shield.png", renderer); menu = loadTexture("images/menu.png", renderer);
    ghost1 = loadTexture("ghost/ghost1.png", renderer); ghost2 = loadTexture("ghost/ghost2.png", renderer);
    angry1 = loadTexture("ghost/angry1.png", renderer); angry2 = loadTexture("ghost/angry2.png", renderer);
    blood = loadTexture("images/blood.jpg", renderer); settings = loadTexture("images/settings.png", renderer);
    dash_particle = loadTexture("images/dash_particle.png", renderer); dash = loadTexture("images/dash.jpg", renderer);
    cd = loadTexture("images/cd.png", renderer); windwall = loadTexture("images/windwall.jpg", renderer);
    Chest = loadTexture("images/chest.png", renderer);
    harm = loadTexture("images/harm.png", renderer); shielded = loadTexture("images/shielded.png", renderer);
    HP = loadTexture("images/HP.png", renderer); heart = loadTexture("images/heart.png", renderer);
    speed = loadTexture("images/speed.png", renderer); speed2 = loadTexture("images/speed2.png", renderer);
    heart2 = loadTexture("images/heart2.png", renderer);
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
}

void draw_effect(){
    char const *p1 = ("Score: " + to_string(score)).c_str();
    SDL_Texture* SCORE = renderText(p1, font50, {236, 221, 34, 255}, renderer);

    renderTexture(SCORE, 242, 533, renderer);

    renderTexture(dash, 5, 520, renderer);
    renderTexture(windwall, 85, 520, renderer);
    renderTexture(blood, 165, 520, renderer);
    renderTexture(heart, 500, 500, renderer);

    if(dShield > 0){
        renderTexture(shield, xPlayer, yPlayer, renderer);
        dShield -= 1.0 / gameSpeed;
    }

    renderTexture(walk[t % 11][isLeft], xPlayer, yPlayer, renderer);

    if(isLeft)renderTexture(book[book_t % 13], xPlayer, yPlayer, renderer);
         else renderTexture(b[book_t % 13], xPlayer, yPlayer, renderer);

    if(book_t) renderTexture(skill[(book_t - 1) % 13], skillhb.x - 75, skillhb.y, renderer);

    if(dSpeed > 0){
        if(t % 8 < 4) renderTexture(speed, xPlayer, yPlayer, renderer);
        else renderTexture(speed2, xPlayer, yPlayer, renderer);
    }

    if(hp > 1) renderTexture(heart, 600, 500, renderer);
          else renderTexture(heart2, 600, 500, renderer);
    if(hp > 2) renderTexture(heart, 700, 500, renderer);
          else renderTexture(heart2, 700, 500, renderer);

    if(dash_cd_rect.w > 0) SDL_RenderFillRect(renderer, &dash_cd_rect);
    if(skill_cd_rect.w > 0) SDL_RenderFillRect(renderer, &skill_cd_rect);
    if(windwall_cd_rect.w > 0) SDL_RenderFillRect(renderer, &windwall_cd_rect);

    while(p.size()){
        auto u = p.back();
        p.pop_back();

        if(u.d > 0){
            renderTexture(u.image, u.x, u.y, renderer);
            u.d -= 1.0 / gameSpeed;
            tmp2.push_back(u);
        }
    }

    swap(tmp2, p);

    SDL_SetRenderDrawColor(renderer, 137, 214, 205, 255);

    if(wall.w > 0){
        if(cnt % 16 == 1) wall.w -= 2, wall.x++;
        SDL_RenderFillRect(renderer, &wall);
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    if(chest) renderTexture(Chest, xChest, land, renderer);
}

void game_over(int &best, bool &isRunning, bool &isMenu){
    SDL_RenderClear(renderer); SDL_Event events;
    Mix_PauseMusic(); play(gameOver);

    SDL_Texture* background = loadTexture("images/over.jpg", renderer);
    SDL_RenderCopy(renderer, background, NULL, NULL);

    best = max(best, score);

    char const *p1 = ("Score: " + to_string(score)).c_str();
    char const *p2 = ("Best: " + to_string(best)).c_str();

    SDL_Texture* SCORE = renderText(p1, font, {236, 221, 34, 255}, renderer);
    SDL_Texture* HIGH = renderText(p2, font, {235, 51, 36, 255}, renderer);

    int tW, tH;
    SDL_QueryTexture(SCORE, NULL, NULL, &tW, &tH);

    SDL_Rect textRect = {(800 - tW) / 2, 420, tW, tH};
    SDL_Rect textRECT = {(800 - tW) / 2, 515, tW, tH};

    SDL_RenderCopy(renderer, SCORE, NULL, &textRect);
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
            ofstream out("best.txt");

            out << best;
            out.close();

            isRunning = 0;
            break;
        }
    }
}
