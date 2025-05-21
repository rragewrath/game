void moveAD(const Uint8* key){
    if(key[SDL_SCANCODE_D]){
        xPlayer += xV / gameSpeed;
        xPlayer = min(xPlayer, 750.0);

        if(isLeft) t = 0;
        isLeft = 0;
    } else
    if(key[SDL_SCANCODE_A]){
        xPlayer -= xV / gameSpeed;
        xPlayer = max(-20.0, xPlayer);

        if(!isLeft) t = 0;
        isLeft = 1;
    } else t = 0;
}

void sword_spawn(){
    if(event <= 0){
        if(rd() % 3 == 1 && !isRise) q.push_back({800, land + 50, 170, 1});

        int x = rd() % 800;

        q.push_back({x, -70, 0, 0});

        int d = rd() % 400 + 40;

        if(x + d <= 800 && rd() % 2) q.push_back({x + d, -70, 0, 0});

        d = rd() % 400 + 40;

        if(x - d >= 0 && rd() % 5 > 1) q.push_back({x - d, -70, 0, 0});

        event = 0.975 + (rd() % max(1000 - score, 300)) / 2000.0;
    }
}

void skill_J(const Uint8* key){
    if(skill_cnt == -1 && key[SDL_SCANCODE_J]){
        skill_cnt = 0;

        skillhb.x = xPlayer + 90 - 105 * (isLeft);
        skillhb.y = yPlayer - 20; skillhb.w = 30;

        if(key[SDL_SCANCODE_S]){
            skillhb.x -= (isLeft * 7 * skillhb.w) / 2;
            skillhb.w *= 4; skill_cd_mode = 1;
        } else skill_cd_mode = 0;

        play(skillSound);

        book_t = 1;
    }
}

void skill_K(const Uint8* key){
    if(windwall_cnt == -1 && key[SDL_SCANCODE_K]){
        play(windwallSound);
        windwall_cnt = 0;

        wall.h = 56; wall.w = 20;

        if(key[SDL_SCANCODE_S]){
            wall.h = 10; wall.w = 65;
            wall.x = xPlayer + 15;
            wall.y = yPlayer - 20;
        } else {
            windwall_cnt = 30;
            if(isLeft){
                wall.x = xPlayer - 5;
                wall.y = yPlayer + 30;
            } else {
                wall.x = xPlayer + 70;
                wall.y = yPlayer + 30;
            }
        }

        ww.push_back(wall);
    }
}

void grid(double lastX, double lastY, vector<SDL_Rect> &blocks){
    bool xFlag = 1, yFlag = 1;

    for(auto &r: blocks){
        for(int i = 0; i < 3; i++){
            if(xFlag){
                SDL_Rect rX = {xPlayer + base_hb[i].x, lastY + base_hb[i].y, base_hb[i].w, base_hb[i].h};
                if(intersect(rX, r)){
                    xPlayer = lastX;
                    xFlag = 0;
                }
            }

            if(yFlag){
                SDL_Rect rY = {lastX + base_hb[i].x, yPlayer + base_hb[i].y, base_hb[i].w, base_hb[i].h};
                if(intersect(rY, r)){
                    if(yV < 0) onGround = 1;
                    yPlayer = lastY;
                    yFlag = 0;
                    yV = 0;
                }
            }
        }
    }
}

void skill_L(const Uint8* key){
    double lastX = xPlayer;
    double lastY = yPlayer;

    if((dash_cnt == -1 || dash_cnt <= 10) && key[SDL_SCANCODE_L]){
        if(dash_cnt == -1) dash_cnt = 0, play(dashSound);

        xLast = xPlayer; yLast = yPlayer;

        if(key[SDL_SCANCODE_S]){
            yV = 1.4 * base;
            yPlayer = min(land, yPlayer - yV / gameSpeed);

            p.push_back({xLast + 20, yLast + 65, 0.05, superjump});
        } else{
            if(isLeft){
                xPlayer -= 13 * xV / gameSpeed;
                xPlayer = max(-20.0, xPlayer);
                p.push_back({xLast, yLast, 0.05, dash_particle[isLeft]});
            } else {
                xPlayer += 13 * xV / gameSpeed;
                xPlayer = min(750.0, xPlayer);
                p.push_back({xLast - 25, yLast, 0.05, dash_particle[isLeft]});
            }
        }
    }

    grid(lastX, lastY, blocks);
    grid(lastX, lastY, ww);
}

void jumping(const Uint8* key){
    if(yPlayer - land){
        yPlayer = min(land, yPlayer - yV / gameSpeed);
        yV -= g / gameSpeed;
    }

    if(onGround && key[SDL_SCANCODE_W]){
        play(jump); onGround = 0;

        yV = base;
        yPlayer = min(land, yPlayer - yV / gameSpeed);
    }
}

void Game::update(){
    if(dSpeed > 0) xV = 160, dSpeed -= 1.0 / gameSpeed; else xV = 70;
    event -= 1.0 / gameSpeed;

    double lastX = xPlayer;
    double lastY = yPlayer;

    const Uint8* key = SDL_GetKeyboardState(NULL);

    moveAD(key); jumping(key); sword_spawn();

    grid(lastX, lastY, blocks);
    grid(lastX, lastY, ww);

    if(yPlayer == land) onGround = 1;

    skill_L(key); skill_J(key); skill_K(key);
}
