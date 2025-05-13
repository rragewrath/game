void Game::update(){
    if(dSpeed > 0) xV = 140, dSpeed -= 1.0 / gameSpeed, g = 200, event += 0.5 / gameSpeed; else xV = 70, g = 260;

    event -= 1.0 / gameSpeed;

    const Uint8* key = SDL_GetKeyboardState(NULL);

    if(yPlayer - land){
        yPlayer = min(land, yPlayer - yV / gameSpeed);
        yV -= g / gameSpeed;
    }

    if(key[SDL_SCANCODE_RIGHT] || key[SDL_SCANCODE_D]){
        xPlayer += xV / gameSpeed;
        xPlayer = min(xPlayer, 750.0);

        if(isLeft) t = 0;
        isLeft = 0;
    } else

    if(key[SDL_SCANCODE_LEFT] || key[SDL_SCANCODE_A]){
        xPlayer -= xV / gameSpeed;
        xPlayer = max(0.0, xPlayer);

        if(!isLeft) t = 0;
        isLeft = 1;
    } else t = 0;

    if(yPlayer == land && (key[SDL_SCANCODE_W] || key[SDL_SCANCODE_UP])){
        play(jump);

        yPlayer = min(land, yPlayer - yV / gameSpeed);
        yV = base - g / gameSpeed;
    }

    if(event <= 0){
        if(rd() % 3 == 1) q.push_back({750, land + 50, 170, 1});

        int x = rd() % 700;

        q.push_back({x + 10, 1, 0, 0});

        int d = rd() % 400 + 40;

        if(x + d <= 710 && rd() % 2){
            q.push_back({x + d, 1, 0, 0});
        }

        d = rd() % 400 + 40;

        if(x - d >= 10 && rd() % 2){
            q.push_back({x - d, 1, 0, 0});
        }

        event = 1.1;
    }

    if((dash_cnt == -1 || (dash_cnt >= 0 && dash_cnt <= 7)) && key[SDL_SCANCODE_L]){
        if(dash_cnt == -1) dash_cnt = 0, play(dashSound);

        xLast = xPlayer;
        yLast = yPlayer;

        if(key[SDL_SCANCODE_S]){
            yV = 1.6 * base - g / gameSpeed;
            yPlayer = min(land, yPlayer - yV / gameSpeed);

            p.push_back({xLast + 20, yLast + 65, 0.05, superjump});
        } else{
            p.push_back({xLast, yLast, 0.05, dash_particle[isLeft]});

            if(isLeft){
                xPlayer -= 20 * xV / gameSpeed;
                xPlayer = max(0.0, xPlayer);
            } else {
                xPlayer += 20 * xV / gameSpeed;
                xPlayer = min(700.0, xPlayer);
            }
        }

    }

    if(skill_cnt == -1 && key[SDL_SCANCODE_J]){
        skill_cnt = 0;

        skillhb.x = xPlayer + 90 - 105 * (isLeft);
        skillhb.y = yPlayer - 20;
        skillhb.w = 30;

        if(key[SDL_SCANCODE_S]){
            skillhb.x -= (isLeft * 7 * skillhb.w) / 2;
            skillhb.w *= 4; skill_cd_mode = 1;
        } else skill_cd_mode = 0;

        play(skillSound);

        book_t = 1;
    }

    if(windwall_cnt == -1 && key[SDL_SCANCODE_K]){
        windwall_cnt = 0;

        play(windwallSound);
        alpha = 255; diff = 1;

        wall.h = 111;
        wall.w = 20;

        if(key[SDL_SCANCODE_S]){
            wall.h = 10; wall.w = 55;
            wall.x = xPlayer + 25;
            wall.y = yPlayer - 20;
        } else {
            diff *= 5;

            if(isLeft){
                wall.x = xPlayer - 5;
                wall.y = yPlayer - 15;
            } else {
                wall.x = xPlayer + 70;
                wall.y = yPlayer - 15;
            }
        }
    }
}
