#include "../include/Player.h"
#include "../include/Game.h"
#include <cmath>

Player::Player() :  vel(480), health(3), jumpTime(0), attackTimeCount(0),
                    jumpCount(0), framesCounter(0), orbCount(0), isJumping(false),
                    ULT(false), attackReady(false), attacking(false),
                    spriteIndex(0), spriteToggle(1), toggleKey(true), texture(nullptr),
                    dubJumpHeight(0.0f)
{
    rec.x = 100;
    rec.y = Constants::SCREEN_HEIGHT/2 - 20;  // Using half the screen height - 20
    rec.width = Constants::UNIT;
    rec.height = Constants::UNIT + 1;
    color = BLACK;
}

void Player::SetTexture(Texture2D *tex) { texture = tex; }

void Player:: Update(float deltaTime, int gameSpeed) {
    if (isJumping)
    {
        if (IsKeyPressed(KEY_A) && jumpCount < 2)
        {
            if (jumpTime <= 1.0f)
            {
                jumpTime = 0;
                dubJumpHeight = 3 * Constants::UNIT;
            }
            else
            {
                dubJumpHeight = rec.y;
                jumpTime = 0;
            }
            jumpCount++;
            // For drawing
            spriteIndex = 0;
            framesCounter = 0;
        }

        jumpTime += deltaTime;
        float yOffset = (vel * jumpTime + 0.5f * Constants::GRAVITY * jumpTime * jumpTime) + dubJumpHeight;
        rec.y = (Constants::SCREEN_HEIGHT/2 - Constants::UNIT) - yOffset;

        if (rec.y >= (Constants::SCREEN_HEIGHT/2 - Constants::UNIT) || IsKeyPressed(KEY_SPACE))
        {
            rec.y = Constants::SCREEN_HEIGHT/2 - Constants::UNIT;
            isJumping = false;
            jumpTime = 0.0f;
            jumpCount = 0;
            dubJumpHeight = 0;
            // For drawing
            if(attacking)
                spriteIndex = 9;
            else
                spriteIndex = 0;
            framesCounter = 0;
        }
    }
    else
    {
        // Toggle between two positions on KEY_SPACE press
        if (IsKeyPressed(KEY_SPACE) && toggleKey)
        {
            rec.y = Constants::SCREEN_HEIGHT/2;
            toggleKey = !toggleKey;
            // For drawing
            if(attacking)
                spriteIndex = 15;
            else
                spriteIndex = 6;
            framesCounter = 0;
        }
        else if (IsKeyPressed(KEY_SPACE) && !toggleKey)
        {
            rec.y = Constants::SCREEN_HEIGHT/2 - Constants::UNIT;
            toggleKey = !toggleKey;
            // For drawing
            if(attacking)
                spriteIndex = 9;
            else
                spriteIndex = 0;
            framesCounter = 0;
        }
    }

    if (IsKeyPressed(KEY_A))
    {
        isJumping = true;
        jumpCount++;
        // For drawing
        spriteIndex = 0;
        framesCounter = 0;
    }

    // Shield (attack) activation
    if (IsKeyPressed(KEY_S) && attackReady)
    {
        attackReady = false;
        attacking = true;
        attackTimeCount = 0;
        orbCount = 0;
        if(rec.y < Constants::SCREEN_HEIGHT/2)
            spriteIndex = 9;
        else
            spriteIndex = 15;
        spriteToggle = 0;
        framesCounter = 0;
    }

    // Shield duration counter
    if(attacking){
        attackTimeCount += deltaTime;
        if(attackTimeCount >= 10){ // MaxAttackTime is 10 seconds
            attacking = false;
            spriteToggle = 1;
            framesCounter = 0;
            attackTimeCount = 0;
        }
    }
}

void Player::Draw() {
    if(texture) {
        if (!attacking)
        {
            if (!isJumping)
            {
                if (rec.y < Constants::SCREEN_HEIGHT/2)
                {
                    framesCounter++;
                    if (framesCounter >= Constants::ANIMATION_SPEED)
                    {
                        spriteIndex++;
                        if (spriteIndex >= 6)
                            spriteIndex = 0;
                        framesCounter = 0;
                    }
                    spriteToggle = 1;
                    DrawTexturePro(*texture, 
                        Rectangle{ static_cast<float>(spriteIndex * _spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Rectangle{ rec.x - 32, rec.y - 44, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Vector2{0, 0}, 0, WHITE);
                }
                else
                {
                    framesCounter++;
                    if (framesCounter >= Constants::ANIMATION_SPEED)
                    {
                        spriteIndex++;
                        if (spriteIndex >= 12)
                            spriteIndex = 6;
                        framesCounter = 0;
                    }
                    spriteToggle = 1;
                    DrawTexturePro(*texture, 
                        Rectangle{ static_cast<float>(spriteIndex * _spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Rectangle{ rec.x - 32, rec.y - 36, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Vector2{0, 0}, 0, WHITE);
                }
            }
            else
            {
                spriteToggle = 1;
                DrawTexturePro(*texture, 
                    Rectangle{ 4 * static_cast<float>(_spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                    Rectangle{ rec.x - 32, rec.y - 44, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                    Vector2{0, 0}, 0, WHITE);
            }
        }
        else
        {
            if (!isJumping)
            {
                if (rec.y < Constants::SCREEN_HEIGHT/2)
                {
                    framesCounter++;
                    if (framesCounter >= Constants::ANIMATION_SPEED)
                    {
                        spriteIndex++;
                        if (spriteIndex >= 15)
                            spriteIndex = 9;
                        framesCounter = 0;
                    }
                    spriteToggle = 0;
                    DrawTexturePro(*texture, 
                        Rectangle{ static_cast<float>(spriteIndex * _spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Rectangle{ rec.x - 32, rec.y - 44, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Vector2{0, 0}, 0, WHITE);
                }
                else
                {
                    framesCounter++;
                    if (framesCounter >= Constants::ANIMATION_SPEED)
                    {
                        spriteIndex++;
                        if (spriteIndex >= 20)
                            spriteIndex = 15;
                        framesCounter = 0;
                    }
                    spriteToggle = 0;
                    DrawTexturePro(*texture, 
                        Rectangle{ static_cast<float>(spriteIndex * _spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Rectangle{ rec.x - 32, rec.y - 36, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                        Vector2{0, 0}, 0, WHITE);
                }
            }
            else
            {
                spriteToggle = 0;
                DrawTexturePro(*texture, 
                    Rectangle{ 12 * static_cast<float>(_spritesWidth), static_cast<float>(spriteToggle * 80), static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                    Rectangle{ rec.x - 32, rec.y - 44, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
                    Vector2{0, 0}, 0, WHITE);
            }
        }
    }
}

void Player::DrawTitle() {
    framesCounter++;
    if (framesCounter >= 40) // ANIMATION SPEED
    {
        spriteIndex++;
        framesCounter = 0;
    }
    if (spriteIndex >= 9)
        spriteIndex = 0;
    DrawTexturePro(*texture, 
        Rectangle{ static_cast<float>(spriteIndex * _spritesWidth), 0, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
        Rectangle{ TITLE_X, TITLE_Y + Constants::UNIT/4, static_cast<float>(_spritesWidth), static_cast<float>(_spritesHeight) },
        Vector2{0, 0}, 0, WHITE);
}