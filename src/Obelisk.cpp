#include "../include/Obelisk.h"
#include "../include/Game.h"

Obelisk::Obelisk() : spriteIndex(0), framesCounter(0), color(BLACK), texture(nullptr)
{
    active = false;
    rec.width = 2 * Constants::UNIT;
    rec.height = 4 * Constants::UNIT;
    rec.x = Constants::SCREEN_WIDTH + 2 * Constants::UNIT;
    rec.y = Constants::SCREEN_HEIGHT/2 - 10 * Constants::UNIT;
}

void Obelisk::SetTexture(Texture2D *tex) { texture = tex; }

void Obelisk::Update(float dt, int gameSpeed) {
    if(active) {
        rec.x -= (5 + gameSpeed);
        if(rec.x < 0)
            active = false;
        framesCounter++;
        if(framesCounter >= Constants::ANIMATION_SPEED) {
            spriteIndex++;
            if(spriteIndex >= 14) spriteIndex = 0;
            framesCounter = 0;
        }
    }
}

void Obelisk::Draw() {
    if(active && texture) {
        Rectangle src = { spriteIndex * 200.0f, 0, 200.0f, 400.0f };
        Rectangle dest = { rec.x, rec.y, rec.width, rec.height };
        DrawTexturePro(*texture, src, dest, {0,0}, 0, WHITE);
    }
}