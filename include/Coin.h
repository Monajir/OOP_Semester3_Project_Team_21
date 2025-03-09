#pragma once
#include "Entity.h"
#include "raylib.h"

class Coin : public Entity {
    friend class Game;
private:
    Vector2 origin;
    float radius;
    int spriteIndex;
    int framesCounter;
    Color color;
    Texture2D* texture;

public:
    Coin();
    void SetTexture(Texture2D* tex);
    void Update(float dt, int gameSpeed) override;
    void Draw() override;
};