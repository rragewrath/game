void ghost_control(){
    if(!isGhost){
        xGhost = rd() % 675 + 75;
        ghosthb.x = xGhost + 4;

        if(rd() % 170 == 0 && abs(xGhost - xPlayer) >= 100) isGhost = 1;

        if(xGhost > xPlayer) goLeft = 0;
        else goLeft = 1;
    } else {
        if(rd() % 17 == 0 && isLeft && !goLeft && xPlayer > xGhost && xPlayer - xGhost <= 100 && cast) goLeft = 1;
        if(rd() % 17 == 0 && !isLeft && goLeft && xPlayer < xGhost && xGhost - xPlayer <= 100 && cast) goLeft = 0;

        if(xGhost == 0) goLeft = 0;
        if(xGhost == 750) goLeft = 1;

        if(!(wall.w > 0 && intersect(ghosthb, wall)))

        if(goLeft){
            xGhost -= ghostV / gameSpeed;
            xGhost = max(0.0, xGhost);
        } else {
            xGhost += ghostV / gameSpeed;
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
    if(!chest && rd() % ((420 * gameSpeed) / 60) == 0){
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

void upd_hb_and_time(){
    if(book_t == 14) cast = book_t = 0;
    if(cast && cnt % 3 == 0) book_t++;

    skillhb.h = 20 + book_t * 5;

    for(int i = 0; i < hb.size(); i++){
        hb[i].h = base_hb[i].h + (dShield > 0) * 20;
        hb[i].w = base_hb[i].w + (dShield > 0) * 20;
        hb[i].x = xPlayer + base_hb[i].x - (dShield > 0) * 10;
        hb[i].y = yPlayer + base_hb[i].y - (dShield > 0) * 8;
    }
}

void open_chest(){
    if(chest && intersect(chest_rect, hb)){
        int chestType = rd() % (13 - 3 * hp);

        if(chestType == 0){
            score += 100;
            play(takescoreSound);
            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, takescore});
        }

        if(chestType >= 4){
            p.push_back({xPlayer, yPlayer, 0.23, HP});
            play(heal); hp = 3;
        }

        if(chestType == 3){
            play(speedboost);
            dSpeed = 8;
        }

        if(chestType == 1) dShield = 6.5;

        if(chestType == 2){
            play(CD); q.clear(); event = 2;
            dash_cd_rect.w = 0;
            skill_cd_rect.w = 0;
            windwall_cd_rect.w = 0;
            p.push_back({xPlayer + 25, yPlayer - 20, 0.20, cd});
        }

        chest = 0;
    }
}

void ghost_attack(){
    if(isGhost && chest && intersect(ghosthb, chest_rect)) chest = 0;

    if(ticks == 80) ghost_resit = 0, ticks = 0;

    if(isGhost && intersect(ghosthb, hb)){
        if(!ghost_resit){
            if(dShield <= 0){
                p.push_back({xPlayer + 25, yPlayer - 20, 0.2, harm});
                hp--; play(harmSound);
            } else {
                p.push_back({xPlayer + 25, yPlayer - 20, 0.2, shielded});
                goLeft ^= 1; play(block);
            }

            ghost_resit = 1;
        } else ticks++;
    } else {
        ghost_resit = 0;
        ticks = 0;
    }
}

void kill_ghost(){
    ghosthb.x = xGhost + 4; ghosthb.y = land + 50;
    ghosthb.w = 22; ghosthb.h = 34;

    if(isGhost && cast && intersect(skillhb, ghosthb)){
        if(angry){
            ghostV /= 3; isGhost = 0; score += 50; angry = 0;
            play(takescoreSound);

            p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killangry});
        } else {
            int t1 = xGhost;

            xGhost = rd() % 675 + 75;
            ghosthb.x = xGhost + 4;

            if(abs(xGhost - xPlayer) <= 200){
                isGhost = 0; score += 20;
                play(takescoreSound);

                p.push_back({xPlayer + 25, yPlayer - 20, 0.23, killghost});
            } else {
                angry = 1;
                ghostV *= 3;

                p.push_back({t1, land, 0.05, dash_particle});
                play(dashSound);

                if(xGhost > xPlayer) goLeft = 0;
                else goLeft = 1;
            }
        }
    }
}

void collision_detection(){
    while(q.size()){
        auto u = q.back();
        q.pop_back();

        if(u.y - land - 20){
            SDL_Rect sw;

            if(!u.t){
                sw.x = u.x+9; sw.y = u.y;
                sw.h = 77; sw.w = 28;
            } else {
                sw.x = u.x + 6; sw.y = u.y + 15;
                sw.h = 20; sw.w = 49;
            }

            if(intersect(sw, hb)){
                if(dShield <= 0){
                    p.push_back({xPlayer + 25, yPlayer - 20, 0.2, harm});
                    play(harmSound); hp--;
                } else {
                    p.push_back({xPlayer + 25, yPlayer - 20, 0.2, shielded});
                    play(block);
                }

                continue;
            }

            u.upd();

            if((!cast || !intersect(skillhb, sw)) && (!intersect(sw, wall) || wall.w <= 0)) tmp.push_back(u);

            if(!u.t) renderTexture(sword, u.x, u.y, renderer);
                else renderTexture(sword2, u.x, u.y, renderer);
        }
    }

    swap(q, tmp);
}
