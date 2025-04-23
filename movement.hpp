
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
        if(rd() % 17 == 0) q.push_back({750, land + 50, 130, 1});

        q.push_back({(rd() + rd() + rd())% 700 + 10, 1, 0, 0});

        event = 0.4;
    }

    if(dash_cd_rect.w <= 0 && key[SDL_SCANCODE_L]){
        dash_cd_rect.x = 30; dash_cd_rect.y = 520;
        dash_cd_rect.h = 56; dash_cd_rect.w = 65;

        xLast = xPlayer;
        yLast = yPlayer;

        p.push_back({xLast, yLast, 0.05, dash_particle});
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
        skill_cd_rect.x = 190; skill_cd_rect.y = 520;
        skill_cd_rect.w = 65; skill_cd_rect.h = 56;

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
