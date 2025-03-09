#include "../include/Coin.h"
#include "../include/Game.h"

Coin::Coin() :  origin({0,0}), radius(7), spriteIndex(0), framesCounter(0),
                color(GREEN), texture(nullptr)
{
    active = false;
}

void Coin::SetTexture(Texture2D *tex) { texture = tex; }

void Coin::Update(float dt, int gameSpeed) {
    if(active) {
        origin.x -= (5 + gameSpeed);
        if(origin.x < 0) {
            active = false;
            spriteIndex = 0;
        }
        framesCounter++;
        if(framesCounter >= Constants::ANIMATION_SPEED) {
            spriteIndex++;
            if(spriteIndex >= 6) spriteIndex = 0;
            framesCounter = 0;
        }
    }
}

void Coin::Draw() {
    if(active && texture) {
        Rectangle src = { 63 + spriteIndex * 16.67f, 32, 16.67f, 18 };
        Rectangle dest = { origin.x - 16.67f/2, origin.y - 18/2, 16.67f, 18 };
        DrawTexturePro(*texture, src, dest, {0,0}, 0, WHITE);
    }
}