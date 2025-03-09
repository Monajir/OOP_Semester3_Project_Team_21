#pragma once
#include "Entity.h"
#include "raylib.h"
#include "Constants.h"
#include "GameScreen.h"

class Game; // Forward declaration

class Player : public Entity {
    friend class Game;
private:
    Rectangle rec;
    Color color;
    float vel;
    float health;
    float jumpTime;
    float attackTimeCount;
    int jumpCount;
    int framesCounter;
    int orbCount;
    bool isJumping;
    bool ULT;
    bool attackReady;
    bool attacking;
    int spriteIndex;
    int spriteToggle;
    bool toggleKey;
    Texture2D *texture;
    const int _spritesHeight = 80;
    const int _spritesWidth = 80;
    float dubJumpHeight;
    static constexpr int TITLE_X = 0;
    static constexpr int TITLE_Y = Constants::SCREEN_HEIGHT/2 - 68;

public:
    Player();
    void SetTexture(Texture2D *tex);
    void Update(float deltaTime, int gameSpeed) override;
    void Draw() override;
    void DrawTitle();
};