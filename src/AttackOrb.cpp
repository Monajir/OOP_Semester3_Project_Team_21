#include "../include/AttackOrb.h"
#include "../include/Game.h"

AttackOrb::AttackOrb() : spriteIndex(0), framesCounter(0), color(PINK), texture(nullptr)
{
    active = false;
    rec.width = Constants::UNIT;
    rec.height = Constants::UNIT;
}

void AttackOrb::SetTexture(Texture2D *tex) { texture = tex; }

void AttackOrb::Update(float dt, int gameSpeed)  {
    if(active) {
        rec.x -= (5 + gameSpeed);
        if(rec.x < 0) {
            active = false;
            spriteIndex = 0;
        }
        framesCounter++;
        if(framesCounter >= Constants::ANIMATION_SPEED) {
            spriteIndex++;
            if(spriteIndex >= 4) spriteIndex = 0;
            framesCounter = 0;
        }
    }
}

void AttackOrb::Draw() {
    if(active && texture) {
        Rectangle src = { 528 + spriteIndex * 16.0f, 208, 16.0f, 17.0f };
        Rectangle dest = { rec.x - 16/1.75f, rec.y - 20, 16 * 2.5f, 17 * 2.5f };
        DrawTexturePro(*texture, src, dest, {0,0}, 0, WHITE);
    }
}