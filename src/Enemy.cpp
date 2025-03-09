#include "../include/Enemy.h"
#include "../include/Game.h"

Enemy::Enemy() :    speed({5,5}), color(GRAY), type(2), spriteIndex(0),
                    framesCounter(0), collisionChecked(false),
                    texType1(nullptr), texType2(nullptr)
{
    rec.width = 10;
    rec.height = Constants::UNIT;
    rec.x = 0;
    rec.y = 0;
    active = false;
}

void Enemy::SetTextures(Texture2D *t1, Texture2D *t2) {
    texType1 = t1;
    texType2 = t2;
}

void Enemy::Update(float dt, int gameSpeed) {
    if(active) {
        rec.x -= (speed.x + gameSpeed);
        if(rec.x < 0)
            active = false;
        framesCounter++;
        if(type == 1) {
            if(framesCounter >= Constants::ANIMATION_SPEED) {
                spriteIndex--;
                if(spriteIndex < 0) spriteIndex = 9;
                framesCounter = 0;
            }
        } else {
            if(framesCounter >= Constants::ANIMATION_SPEED) {
                spriteIndex++;
                if(spriteIndex >= 10) spriteIndex = 0;
                framesCounter = 0;
            }
        }
    }
}

void Enemy::Draw() {
    if(active && texType1 && texType2) {
        if(type == 1) {
            Rectangle src = { static_cast<float>(spriteIndex * 40), 0, 40, 32 };
            Rectangle dest = { rec.x - 15, rec.y - 15, 40 * 1.5f, 32 * 1.5f };
            DrawTexturePro(*texType1, src, dest, {0,0}, 0, WHITE);
        }
        else {
            if(rec.y >= Constants::SCREEN_HEIGHT/2) {
                Rectangle src = { 0, 64, 48, 64 };
                Rectangle dest = { rec.x - 15, rec.y - Constants::UNIT, 48, 64/1.5f };
                DrawTexturePro(*texType2, src, dest, {0,0}, 0, WHITE);
                Rectangle src2 = { static_cast<float>(spriteIndex * 48), 64, 48, 64 };
                DrawTexturePro(*texType2, src2, dest, {0,0}, 0, WHITE);
            }
            else {
                Rectangle src = { 0, 0, 48, 64 };
                Rectangle dest = { rec.x - 15, rec.y - Constants::UNIT - 3, 48, 64/1.5f };
                DrawTexturePro(*texType2, src, dest, {0,0}, 0, WHITE);
                Rectangle src2 = { static_cast<float>(spriteIndex * 48), 0, 48, 64 };
                DrawTexturePro(*texType2, src2, dest, {0,0}, 0, WHITE);
            }
        }
    }
}