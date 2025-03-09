#include "../include/Game.h"

Game::Game() :  currentScreen(LOGO), scrollingOne(0), scrollingTwo(0), scrollingThree(0),
                scrollingFour(0), scrollingFive(0), scrollingSix(0), phaseCounter(0),
                enemyTimeCounter(0), maxEnemyDiff(1), coinTimeCounter(0), maxCoinDiff(2),
                heartTimeCounter(0), maxHeartDiff(15.5f), obeliskTimeCounter(0),
                maxObeliskTime(20), attackOrbTimeCounter(0), maxAttOrbTime(6.5f),
                maxAttackTime(10), gameSpeed(0), coinCount(0), heartCount(0)
{
    startRect = {600, 180, static_cast<float>(screenWidth/6), 30};
    exitRect = {600, 300, static_cast<float>(screenWidth/6), 30};
    scoreBoardRect = {600, 240, static_cast<float>(screenWidth/6), 30}; 
    restartRect = { static_cast<float>(screenWidth/2 - 4*Constants::UNIT),
            static_cast<float>(screenHeight/2 + 2*Constants::UNIT + 60),
            static_cast<float>(screenWidth/6 + 2*Constants::UNIT), 30 };

    enemies.resize(Constants::MAX_ENEMY);
    coins.resize(Constants::MAX_COIN);
    hearts.resize(Constants::MAX_HEART);
    attackOrbs.resize(Constants::MAX_ATTACK_ORB);
}

void Game::LoadScoreboard() {
    scoreboard.clear();
    std::ifstream in(scoreFile);
    if(in.is_open()){
        std::string line;
        while(std::getline(in, line)){
            std::istringstream iss(line);
            std::string name;
            int score;
            if(iss >> name >> score){
                scoreboard.push_back({name, score});
            }
        }
        in.close();
    }
}

void Game::SaveScoreboard() {
    std::ofstream out(scoreFile, std::ofstream::trunc);
    if(out.is_open()){
        for(auto &entry : scoreboard){
            out << entry.name << " " << entry.score << "\n";
        }
        out.close();
    }
}

void Game::UpdateScoreboardRecord() {
    ScoreEntry entry { playerName, coinCount };
    scoreboard.push_back(entry);
    std::sort(scoreboard.begin(), scoreboard.end(), [](const ScoreEntry &a, const ScoreEntry &b) {
        return a.score > b.score;
    });
    if(scoreboard.size() > 5)
        scoreboard.resize(5);
    SaveScoreboard();
}

void Game::Init() {
    SetConfigFlags(FLAG_FULLSCREEN_MODE); // For Full Screen Mode. Kinda forcing full screen
    InitWindow(screenWidth, screenHeight, "Amaterasu OOP Game");
    
    // Load textures
    playerTexture = LoadTexture("Assests/NightBorne/NightBorne.png");
    enemyTexture1 = LoadTexture("Assests/Dark VFX 1/DarkVFX(Rev).png");
    enemyTexture2 = LoadTexture("Assests/Dark VFX 2/Combined.png");
    coinsHeartsTexture = LoadTexture("Assests/Coins/coins-chests-etc-2-0.png");
    obeliskTexture = LoadTexture("Assests/Obelisk/FlyingObelisk.png");
    
    // Load background textures
    bgOne = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/1.png");
    bgTwo = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/2.png");
    bgThree = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/3.png");
    bgFour = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/4.png");
    bgFive = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/5.png");
    bgSix = LoadTexture("Assests/Parallex Bgs/Clouds prallex Bg/Clouds 8/6.png");
    
    // Set textures for entities
    player.SetTexture(&playerTexture);
    for(auto &enemy : enemies) {
        enemy.SetTextures(&enemyTexture1, &enemyTexture2);
    }
    for(auto &coin : coins) {
        coin.SetTexture(&coinsHeartsTexture);
    }
    for(auto &heart : hearts) {
        heart.SetTexture(&coinsHeartsTexture);
    }
    for(auto &orb : attackOrbs) {
        orb.SetTexture(&coinsHeartsTexture);
    }
    obelisk.SetTexture(&obeliskTexture);
    
    SetTargetFPS(60);
    LoadScoreboard();
    InitGameObjects();
}

void Game::InitGameObjects() {
    // Reset Player state
    player.rec.x = 100;
    player.rec.y = screenHeight/2 - Constants::UNIT;
    player.rec.width = Constants::UNIT;
    player.rec.height = Constants::UNIT + 1;
    player.jumpCount = 0;
    player.isJumping = false;
    player.color = BLACK;
    player.vel = 480;
    player.jumpTime = 0;
    player.framesCounter = 0;
    player.health = 3;
    player.orbCount = 0;
    player.ULT = false;
    player.attackReady = false;
    player.attacking = false;
    player.attackTimeCount = 0;
    player.spriteIndex = 0;
    player.spriteToggle = 1;
    player.toggleKey = true;
    player.dubJumpHeight = 0.0f;
    
    // Reset Enemies
    for(auto &enemy : enemies) {
        enemy.rec.width = 10;
        enemy.rec.height = Constants::UNIT;
        enemy.speed = {5,5};
        enemy.color = GRAY;
        enemy.collisionChecked = false;
        enemy.active = false;
    }
    
    // Reset Coins
    for(auto &coin : coins) {
        coin.radius = 7;
        coin.color = GREEN;
        coin.active = false;
    }
    
    // Reset Hearts
    heartCount = 0;
    for(auto &heart : hearts) {
        heart.rec.width = Constants::UNIT;
        heart.rec.height = 1.5f * Constants::UNIT;
        heart.color = RED;
        heart.active = false;
    }
    
    // Reset Obelisk
    obelisk.active = false;
    obelisk.rec.width = 2 * Constants::UNIT;
    obelisk.rec.height = 4 * Constants::UNIT;
    obelisk.rec.x = screenWidth + 2 * Constants::UNIT;
    obelisk.rec.y = screenHeight/2 - 10 * Constants::UNIT;
    obelisk.color = BLACK;
    obelisk.spriteIndex = 0;
    
    // Reset Attack Orbs
    for(auto &orb : attackOrbs) {
        orb.active = false;
        orb.rec.width = Constants::UNIT;
        orb.rec.height = Constants::UNIT;
        orb.color = PINK;
    }
    
    coinCount = 0;
    phaseCounter = 0;
    enemyTimeCounter = 0;
    attackOrbTimeCounter = 0;
    heartTimeCounter = 0;
    coinTimeCounter = 0;
    obeliskTimeCounter = 0;
    scrollingOne = scrollingTwo = scrollingThree = scrollingFour = scrollingFive = scrollingSix = 0;
    gameSpeed = 0;


    // (Player name is set on the Name Input screen.)
    scoreRecorded = false;
}

void Game::Run() {
    while(!WindowShouldClose()) {
        switch(currentScreen) {
            case LOGO:
                if(++phaseCounter > 120) {
                    currentScreen = TITLE;
                    phaseCounter = 0;
                }
                break;
            case TITLE: {
                mousePos = GetMousePosition();
                // if(CheckCollisionPointRec(mousePos, startRect)) {
                //     if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                //         currentScreen = GAMEPLAY;
                //         phaseCounter = 0;
                //         player.framesCounter = 0;
                //     }
                // }

                if(CheckCollisionPointRec(mousePos, startRect)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        currentScreen = NAME_INPUT;
                    }
                }

                if(CheckCollisionPointRec(mousePos, scoreBoardRect)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        currentScreen = SCOREBOARD;
                    }
                }

                if(CheckCollisionPointRec(mousePos, exitRect)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        currentScreen = ENDING;
                        phaseCounter = 0;
                    }
                }
                if(++phaseCounter >= Constants::ANIMATION_SPEED) {
                    player.spriteIndex++;
                    phaseCounter = 0;
                }
                if(player.spriteIndex >= 9) player.spriteIndex = 0;

                // Scrolling effect update
                scrollingOne -= 0.015f;
                scrollingTwo -= 0.05f;
                scrollingThree -= 0.115f;
                scrollingFour -= 0.17f;
                scrollingFive -= 0.13f;
                scrollingSix -= 0.015f;
                if(scrollingOne <= -bgOne.width * 1.38f) scrollingOne = 0;
                if(scrollingTwo <= -bgTwo.width * 1.38f) scrollingTwo = 0;
                if(scrollingThree <= -bgThree.width * 1.38f) scrollingThree = 0;
                if(scrollingFour <= -bgOne.width * 1.38f) scrollingFour = 0;
                if(scrollingFive <= -bgTwo.width * 1.38f) scrollingFive = 0;
                if(scrollingSix <= -bgThree.width * 1.38f) scrollingSix = 0;

                break;
            }
            case NAME_INPUT: {
                // Handle text input for player name
                int key = GetCharPressed();
                while(key > 0) {
                    if((key >= 32) && (key <= 125) && (playerName.size() < 20))
                        playerName.push_back((char)key);
                    key = GetCharPressed();
                }
                if(IsKeyPressed(KEY_BACKSPACE) && !playerName.empty())
                    playerName.pop_back();
                // If player presses ENTER and name is not empty, go to gameplay
                if(IsKeyPressed(KEY_ENTER) && !playerName.empty()){
                    currentScreen = GAMEPLAY;
                }
                mousePos = GetMousePosition();
                Rectangle backRect = {600, 300, static_cast<float>(screenWidth/6), 30};
                if(CheckCollisionPointRec(mousePos, backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    currentScreen = TITLE;
                break;
            }
            case GAMEPLAY:
                deltaTime = GetFrameTime();
                phaseCounter += deltaTime;
                if(phaseCounter <= Constants::FIRST_PHASE) {
                    maxEnemyDiff = 1.0f;
                    gameSpeed = 1;
                } else if(phaseCounter <= Constants::SECOND_PHASE) {
                    maxEnemyDiff = 0.75f;
                } else if(phaseCounter <= Constants::THIRD_PHASE) {
                    maxEnemyDiff = 0.50f;
                    gameSpeed = 2;
                } else {
                    maxEnemyDiff = 0.40f;
                    gameSpeed = 4;
                }

                UpdateGame();
                
                scrollingOne -= 0.1f;
                scrollingTwo -= 0.25f;
                scrollingThree -= 0.5f;
                scrollingFour -= 0.75f;
                scrollingFive -= 1.0f;
                scrollingSix -= 0.1f;
                if(scrollingOne <= -bgOne.width * 1.38f) scrollingOne = 0;
                if(scrollingTwo <= -bgTwo.width * 1.38f) scrollingTwo = 0;
                if(scrollingThree <= -bgThree.width * 1.38f) scrollingThree = 0;
                if(scrollingFour <= -bgOne.width * 1.38f) scrollingFour = 0;
                if(scrollingFive <= -bgTwo.width * 1.38f) scrollingFive = 0;
                if(scrollingSix <= -bgThree.width * 1.38f) scrollingSix = 0;
                break;
            case SCOREBOARD: {
                    mousePos = GetMousePosition();
                    // Back button in Scoreboard screen
                    Rectangle backRect = {600, 300, static_cast<float>(screenWidth/6), 30};
                    if(CheckCollisionPointRec(mousePos, backRect) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        currentScreen = TITLE;
                    break;
            }
            case GAMEOVER:
                mousePos = GetMousePosition();
                if(CheckCollisionPointRec(mousePos, restartRect)) {
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        InitGameObjects();
                        playerName.clear();
                        currentScreen = NAME_INPUT;
                    }
                }
                break;
            case ENDING:
                if(++phaseCounter > 120) {
                    CloseWindow();
                    return;
                }
                break;
        }
        
        BeginDrawing();
        ClearBackground(RAYWHITE);
        switch(currentScreen) {
            case LOGO:
                DrawText("AMATERASU", screenWidth/2 - 3*40, screenHeight/2 - 40, 40, LIGHTGRAY);
                DrawText("Loading...", 280, 250, 20, GRAY);
                break;
            case TITLE:
                DrawTextureEx(bgOne, { scrollingOne, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgOne, { static_cast<float>(bgOne.width)*1.38f + scrollingOne, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgTwo, { scrollingTwo, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgTwo, { static_cast<float>(bgTwo.width)*1.38f + scrollingTwo, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgThree, { scrollingThree, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgThree, { static_cast<float>(bgThree.width)*1.38f + scrollingThree, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgFive, { scrollingFive, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgFive, { static_cast<float>(bgTwo.width)*1.38f + scrollingFive, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgSix, { scrollingSix, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgSix, { static_cast<float>(bgThree.width)*1.38f + scrollingSix, 0 }, 0.0f, 1.38f, WHITE);
                
                DrawLineEx({0, static_cast<float>(screenHeight)/2}, { static_cast<float>(screenWidth), static_cast<float>(screenHeight)/2}, 2.0f, BLACK);
                DrawRectangle(600, 100, screenWidth/6, 30, BLACK);
                DrawText("START", 615, 100, 30, WHITE);
                
                DrawRectangle(600, 160, screenWidth/6, 30, BLACK);
                DrawText("SCORE", 615, 160, 30, WHITE);
                
                DrawRectangle(600, 220, screenWidth/6, 30, BLACK);
                DrawText("EXIT", 630, 220, 30, WHITE);
                player.DrawTitle();
                break;
            case NAME_INPUT:
                DrawText("Enter your name:", 50, 50, 20, BLACK);
                DrawRectangle(50, 80, 300, 30, LIGHTGRAY);
                DrawText(playerName.c_str(), 60, 85, 20, BLACK);
                DrawText("Press ENTER to continue", 50, 120, 20, GRAY);
                // Draw Back button
                {
                    Rectangle backRect = {600, 220, static_cast<float>(screenWidth/6), 30};
                    DrawRectangle(backRect.x, backRect.y, backRect.width, backRect.height, BLACK);
                    DrawText("Back", backRect.x + 15, backRect.y, 30, WHITE);
                }
                break;
            case GAMEPLAY:
                DrawTextureEx(bgOne, { scrollingOne, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgOne, { static_cast<float>(bgOne.width)*1.38f + scrollingOne, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgTwo, { scrollingTwo, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgTwo, { static_cast<float>(bgTwo.width)*1.38f + scrollingTwo, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgThree, { scrollingThree, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgThree, { static_cast<float>(bgThree.width)*1.38f + scrollingThree, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgFive, { scrollingFive, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgFive, { static_cast<float>(bgTwo.width)*1.38f + scrollingFive, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgSix, { scrollingSix, 0 }, 0.0f, 1.38f, WHITE);
                DrawTextureEx(bgSix, { static_cast<float>(bgThree.width)*1.38f + scrollingSix, 0 }, 0.0f, 1.38f, WHITE);
                
                DrawLineEx({0, static_cast<float>(screenHeight)/2}, { static_cast<float>(screenWidth), static_cast<float>(screenHeight)/2}, 2.0f, BLACK);
                DrawText(TextFormat("PLAYER HEALTH: %.1f", player.health), Constants::UNIT, Constants::UNIT, Constants::UNIT, BLACK);
                DrawText(TextFormat("COIN COUNT : %d", coinCount), Constants::UNIT, Constants::UNIT*3, Constants::UNIT, BLACK);
                DrawText(TextFormat("ULT : %s", (player.ULT) ? "Ready" : "NOT Ready"), screenWidth/2, Constants::UNIT, Constants::UNIT, BLACK);
                DrawText(TextFormat("ORB COUNT : %d", player.orbCount), screenWidth/2 - 5*Constants::UNIT, Constants::UNIT*3, Constants::UNIT, BLACK);
                DrawText(TextFormat("ATTACK : %s", (player.orbCount == Constants::MAX_REQ_ATT_ORB) ? "Ready" : "NOT Ready"), screenWidth*0.65, Constants::UNIT*3, Constants::UNIT, BLACK);
                
                player.Draw();
                for(auto &enemy : enemies) { if(enemy.active) enemy.Draw(); }
                for(auto &coin : coins) { if(coin.active) coin.Draw(); }
                for(auto &heart : hearts) { if(heart.active) heart.Draw(); }
                obelisk.Draw();
                for(auto &orb : attackOrbs) { if(orb.active) orb.Draw(); }
                break;
            case SCOREBOARD: {
                    DrawText("Scoreboard", 50, 50, 30, BLACK);
                    int yOffset = 100;
                    for (int i = 0; i < scoreboard.size() && i < 5; i++) {
                        std::string entry = std::to_string(i+1) + ". " + scoreboard[i].name + " - " + std::to_string(scoreboard[i].score);
                        DrawText(entry.c_str(), 50, yOffset, 20, BLACK);
                        yOffset += 30;
                    }
                    // Draw Back button
                    {
                        Rectangle backRect = {600, 220, static_cast<float>(screenWidth/6), 30};
                        DrawRectangle(backRect.x, backRect.y, backRect.width, backRect.height, BLACK);
                        DrawText("Back", backRect.x + 15, backRect.y, 30, WHITE);
                    }
                    break;
                }
            case GAMEOVER:
                {
                    // Rectangle restartRect = { static_cast<float>(screenWidth/2 - 4*Constants::UNIT),
                    //                           static_cast<float>(screenHeight/2 + 2*Constants::UNIT),
                    //                           static_cast<float>(screenWidth/6 + 2*Constants::UNIT), 30 };
                    DrawRectangle(restartRect.x, restartRect.y - 60, restartRect.width, restartRect.height, BLACK);
                    DrawText("RESTART", restartRect.x + 15, restartRect.y - 60, 30, WHITE);
                    DrawText("Game Over", screenWidth/2 - 5*Constants::UNIT, screenHeight/2 - 2*Constants::UNIT, 2*Constants::UNIT, BLACK);
                }
                break;
            case ENDING:
                DrawRectangle(0, 0, screenWidth, screenHeight, WHITE);
                DrawText("CLOSING THE GAME", 20, 20, 40, GRAY);
                break;
        }
        EndDrawing();
    }
    
    UnloadTexture(bgOne);
    UnloadTexture(bgTwo);
    UnloadTexture(bgThree);
    UnloadTexture(bgFour);
    UnloadTexture(bgFive);
    UnloadTexture(bgSix);
    UnloadTexture(playerTexture);
    UnloadTexture(coinsHeartsTexture);
    UnloadTexture(obeliskTexture);
    UnloadTexture(enemyTexture1);
    UnloadTexture(enemyTexture2);
    CloseWindow();
}

void Game::UpdateGame() {
    player.Update(deltaTime, gameSpeed);
    if(player.health <= 0) {
        if(!scoreRecorded) {
            UpdateScoreboardRecord();
            scoreRecorded = true;
        }
        currentScreen = GAMEOVER;
    }
    
    enemyTimeCounter += deltaTime;
    if(enemyTimeCounter >= maxEnemyDiff) {
        for(auto &enemy : enemies) {
            if(!enemy.active) {
                enemy.active = true;
                enemy.collisionChecked = false;
                enemy.rec.x = screenWidth;
                int randHeight = GetRandomValue(1,3);
                enemy.type = 2;
                int randDir = GetRandomValue(0,1);
                if(randDir == 0) {
                    randDir = -1;
                    enemy.rec.y = screenHeight/2 - 20 * randDir;
                } else {
                    enemy.rec.y = screenHeight/2 - (randHeight) * 20 * randDir;
                    enemy.rec.height = Constants::UNIT;
                    if(randHeight != 1) {
                        enemy.type = 1;
                        enemy.spriteIndex = 9;
                        enemy.rec.height = Constants::UNIT + 1;
                    }
                }
                enemy.spriteIndex = 0;
                
                maxCoinDiff = GetRandomValue(2,4);
                coinTimeCounter++;
                if(coinTimeCounter >= maxCoinDiff) {
                    int fiveCounter = 0;
                    for(auto &coin : coins) {
                        if(!coin.active) {
                            coin.active = true;
                            coin.origin.x = screenWidth + fiveCounter * 10 * (gameSpeed+1) - 4 * Constants::UNIT;
                            if((randHeight == 1 || randHeight == 2) && randDir != -1)
                                coin.origin.y = screenHeight/2 - Constants::UNIT/4 - (Constants::UNIT * fiveCounter * 2 - 0.5f * (0.5f * Constants::UNIT) * fiveCounter * fiveCounter);
                            else
                                coin.origin.y = screenHeight/2 - Constants::UNIT/2;
                            fiveCounter++;
                        }
                        if(fiveCounter == (int)ceil(9 * maxEnemyDiff))
                            break;
                    }
                    coinTimeCounter = 0;
                }
                break;
            }
        }
        enemyTimeCounter = 0;
    }
    
    for(auto &enemy : enemies) { if(enemy.active) enemy.Update(deltaTime, gameSpeed); }
    for(auto &coin : coins) { if(coin.active) coin.Update(deltaTime, gameSpeed); }
    
    heartTimeCounter += deltaTime;
    if(heartTimeCounter >= maxHeartDiff && heartCount <= Constants::MAX_HEART) {
        for(auto &heart : hearts) {
            if(!heart.active) {
                heart.active = true;
                heart.rec.x = screenWidth;
                heart.rec.y = screenHeight/2 - 5 * Constants::UNIT;
                heart.spriteIndex = 0;
                heartCount++;
                break;
            }
        }
        heartTimeCounter = 0;
    }
    if(heartCount <= Constants::MAX_HEART) {
        for(auto &heart : hearts) { if(heart.active) heart.Update(deltaTime, gameSpeed); }
    }
    
    if(obeliskTimeCounter < maxObeliskTime)
        obeliskTimeCounter += deltaTime;
    else if(!obelisk.active)
        obelisk.active = true;
    if(obelisk.active)
        obelisk.Update(deltaTime, gameSpeed);
    
    attackOrbTimeCounter += deltaTime;
    if(attackOrbTimeCounter >= maxAttOrbTime) {
        for(auto &orb : attackOrbs) {
            if(!orb.active) {
                orb.active = true;
                orb.rec.x = screenWidth;
                orb.rec.y = screenHeight/2 - 6 * Constants::UNIT;
                orb.spriteIndex = 0;
                break;
            }
        }
        attackOrbTimeCounter = 0;
    }
    for(auto &orb : attackOrbs) { if(orb.active) orb.Update(deltaTime, gameSpeed); }
    
    if(!player.attacking) {
        for(auto &enemy : enemies) {
            if(enemy.active && !enemy.collisionChecked && CheckCollisionRecs(player.rec, enemy.rec)) {
                player.health -= 0.5f;
                enemy.collisionChecked = true;
            }
        }
    }
    for(auto &coin : coins) {
        if(coin.active && CheckCollisionCircleRec(coin.origin, coin.radius, player.rec)) {
            coinCount++;
            coin.active = false;
            coin.spriteIndex = 0;
        }
    }
    for(auto &heart : hearts) {
        if(heart.active && CheckCollisionRecs(player.rec, heart.rec)) {
            player.health += 1;
            heart.active = false;
        }
    }
    if(CheckCollisionRecs(player.rec, obelisk.rec) && obelisk.active) {
        player.ULT = true;
        obelisk.active = false;
    }
    if(!player.attacking) {
        for(auto &orb : attackOrbs) {
            if(orb.active && player.orbCount < Constants::MAX_REQ_ATT_ORB && CheckCollisionRecs(player.rec, orb.rec)) {
                orb.active = false;
                player.orbCount++;
            }
            if(player.orbCount == Constants::MAX_REQ_ATT_ORB)
                player.attackReady = true;
        }
    }
}
