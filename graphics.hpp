void cdr(){
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
}

void loadAll(SDL_Renderer* renderer){
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

void draw_effect(SDL_Renderer *renderer){
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

    if(hp > 1) renderTexture(heart, 600, 500, renderer);
          else renderTexture(heart2, 600, 500, renderer);
    if(hp > 2) renderTexture(heart, 700, 500, renderer);
          else renderTexture(heart2, 700, 500, renderer);

    if(dash_cd_rect.w > 0) SDL_RenderFillRect(renderer, &dash_cd_rect);
    if(skill_cd_rect.w > 0) SDL_RenderFillRect(renderer, &skill_cd_rect);
    if(windwall_cd_rect.w > 0) SDL_RenderFillRect(renderer, &windwall_cd_rect);
}
