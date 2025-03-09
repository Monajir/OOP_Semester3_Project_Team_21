#pragma once
#include "Entity.h"
#include "raylib.h"
#include "Constants.h"

class AttackOrb : public Entity {
    friend class Game;
private:
    Rectangle rec;
    int spriteIndex;
    int framesCounter;
    Color color;
    Texture2D* texture;

public:
    AttackOrb();
    void SetTexture(Texture2D* tex);
    void Update(float dt, int gameSpeed) override;
    void Draw() override;
};
