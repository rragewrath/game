void ghost_control(SDL_Renderer *renderer){
    if(!isGhost){
        xGhost = rd() % 675 + 75;
        ghosthb.x = xGhost + 4;

        if(rd() % 170 == 0 && abs(xGhost - xPlayer) >= 100) isGhost = 1;

        if(xGhost > xPlayer) goLeft = 0;
        else goLeft = 1;
    } else {
        if(isLeft && !goLeft && xPlayer > xGhost && xPlayer - xGhost <= 100 && cast) goLeft = 1;
        if(!isLeft && goLeft && xPlayer < xGhost && xGhost - xPlayer <= 100 && cast) goLeft = 0;

        if(xGhost == 0) goLeft = 0;
        if(xGhost == 750) goLeft = 1;

        if(!(wall.w > 0 && intersect(ghosthb, wall)))

        if(goLeft){
            xGhost -= ghostV / FPS;
            xGhost = max(0.0, xGhost);
        } else {
            xGhost += ghostV / FPS;
            xGhost = min(xGhost, 750.0);
        }

        if(cnt % 60 < 30){
            if(angry) renderTexture(angry1, xGhost, land, renderer);
                 else renderTexture(ghost1, xGhost, land, renderer);
        } else {
            if(angry) renderTexture(angry2, xGhost, land, renderer);
                 else renderTexture(ghost2, xGhost, land, renderer);
        }
    }
}

void add_chest(){
    if(!chest && rd() % 500 == 0){
        xChest = rd() % 650 + 75;

        chest_rect.y = land + 55;
        chest_rect.x = xChest;
        chest_rect.h = 32;
        chest_rect.w = 42;

        while(abs(xChest - xPlayer) <= 30){
            xChest = rd() % 650 + 75;
            chest_rect.x = xChest;
        }

        chest = 1;
    }
}

void open_chest(){
    if(chest && intersect(chest_rect, hb)){
        int chestType = rd() % (4 + (hp != 3));

        if(chestType == 0){
            score += 100;
            play(takescoreSound);
            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, takescore});
        }

        if(chestType == 4){
            p.push_back({xPlayer, yPlayer, 0.23, HP});
            play(heal); hp = 3;    
        }

        if(chestType == 3){
            play(speedboost);
            dSpeed = 5;
        }

        if(chestType == 1) dShield = 4;

        if(chestType == 2){
            play(CD); q.clear(); event = 1;
            dash_cd_rect.w = 0;
            skill_cd_rect.w = 0;
            windwall_cd_rect.w = 0;
            p.push_back({xPlayer + 25, yPlayer - 20, 0.20, cd});
        }

        chest = 0;
    }
}

void remove_chest(){
    if(isGhost && chest && intersect(ghosthb, chest_rect)) chest = 0;
    if(isGhost && intersect(ghosthb, hb)){
        if(dShield <= 0){
            p.push_back({xPlayer + 25, yPlayer - 20, 0.2, harm});
            hp--; play(harmSound);
        } else {
            p.push_back({xPlayer + 25, yPlayer - 20, 0.2, shielded});
            play(block);
        }
        goLeft ^= 1;

        if(xGhost + 20 >= 750) xGhost -= 14; else xGhost += 14;
    }
}

void kill_ghost(){
    hb.x = xPlayer + 29 - (dShield > 0) * 10;
    hb.y = yPlayer + 29 - (dShield > 0) * 8;

    ghosthb.x = xGhost + 4;
    ghosthb.y = land + 50;
    ghosthb.w = 22;
    ghosthb.h = 34;

    if(isGhost && cast && intersect(skillhb, ghosthb)){
        if(angry){
            ghostV /= 3; isGhost = 0; score += 50; angry = 0;
            play(takescoreSound);

            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killangry});
        } else {
            int t1 = xGhost;

            xGhost = rd() % 675 + 75;
            ghosthb.x = xGhost + 4;

            if(abs(xGhost - xPlayer) <= 250){
                isGhost = 0; score += 20;
                play(takescoreSound);

                p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killghost});
            } else {
                angry = 1;
                ghostV *= 3;

                p.push_back({t1, land, 0.15, dash_particle});
                play(dashSound);

                if(xGhost > xPlayer) goLeft = 0;
                else goLeft = 1;
            }
        }
    }
}
