#pragma once
#include "Entity.h"
#include "raylib.h"

class Game; // Forward declaration

class Enemy : public Entity {
    friend class Game;
private:
    Rectangle rec;
    Vector2 speed;
    Color color;
    int type;
    int spriteIndex;
    int framesCounter;
    bool collisionChecked;
    Texture2D *texType1;
    Texture2D *texType2;

public:
    Enemy();
    void SetTextures(Texture2D *t1, Texture2D *t2);
    void Update(float dt, int gameSpeed) override;
    void Draw() override;
};