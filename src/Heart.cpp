#include "../include/Heart.h"
#include "../include/Game.h"

Heart::Heart() : spriteIndex(0), framesCounter(0), color(RED), texture(nullptr)
{
    active = false;
    rec.width = Constants::UNIT;
    rec.height = 1.5f * Constants::UNIT;
}

void Heart::SetTexture(Texture2D *tex) { texture = tex; }

void Heart::Update(float dt, int gameSpeed) {
    if(active) {
        rec.x -= (5 + gameSpeed);
        if(rec.x < 0) active = false;
        framesCounter++;
        if(framesCounter >= Constants::ANIMATION_SPEED) {
            spriteIndex++;
            if(spriteIndex >= 4) spriteIndex = 0;
            framesCounter = 0;
        }
    }
}

void Heart::Draw() {
    if(active && texture) {
        Rectangle src = { 528 + spriteIndex * 16.0f, 240, 16.0f, 17.0f };
        Rectangle dest = { rec.x - 16/1.75f, rec.y - 20, 16 * 2.5f, 17 * 2.5f };
        DrawTexturePro(*texture, src, dest, {0,0}, 0, WHITE);
    }
}