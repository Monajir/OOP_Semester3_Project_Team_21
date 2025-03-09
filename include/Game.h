#pragma once
#include "raylib.h"
#include "Entity.h"
#include "Player.h"
#include "Enemy.h"
#include "Coin.h"
#include "Heart.h"
#include "Obelisk.h"
#include "AttackOrb.h"
#include "Constants.h"
#include "GameScreen.h"
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <fstream>

struct ScoreEntry {
    std::string name;
    int score;
};

class Game {
private:
    const int screenWidth = Constants::SCREEN_WIDTH;
    const int screenHeight = Constants::SCREEN_HEIGHT;
    
    GameScreen currentScreen;
    
    // Background scrolling positions and textures
    float scrollingOne, scrollingTwo, scrollingThree, scrollingFour, scrollingFive, scrollingSix;
    Texture2D bgOne, bgTwo, bgThree, bgFour, bgFive, bgSix;
    
    // Timing variables
    float deltaTime;
    float phaseCounter;
    float enemyTimeCounter;
    float maxEnemyDiff;
    float coinTimeCounter;
    float maxCoinDiff;
    float heartTimeCounter;
    float maxHeartDiff;
    float obeliskTimeCounter;
    float maxObeliskTime;
    float attackOrbTimeCounter;
    float maxAttOrbTime;
    float maxAttackTime;
    
    int gameSpeed;
    int coinCount;
    int heartCount;
    
    // UI Rectangles
    Rectangle startRect, exitRect, restartRect, scoreBoardRect;
    Vector2 mousePos;
    
    // Game Entities
    Player player;
    std::vector<Enemy> enemies;
    std::vector<Coin> coins;
    std::vector<Heart> hearts;
    Obelisk obelisk;
    std::vector<AttackOrb> attackOrbs;
    
    // Textures for entities
    Texture2D playerTexture;
    Texture2D enemyTexture1;
    Texture2D enemyTexture2;
    Texture2D coinsHeartsTexture;
    Texture2D obeliskTexture;

    // Music 

    Music titleMusic;
    Music gameplayMusic;
    Music gameOverMusic;

    std::string playerName;
    bool scoreRecorded;
    std::vector<ScoreEntry> scoreboard; // Holds all score records

    // File to store scoreboard data
    const std::string scoreFile = "scoreboard.txt";
public:
    Game();
    void Init();
    void Run();
    void LoadScoreboard();
    void SaveScoreboard();
    void UpdateScoreboardRecord();
    void InitGameObjects();
    void UpdateGame();
    void UpdateMusic();
};