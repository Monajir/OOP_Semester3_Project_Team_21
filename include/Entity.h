#pragma once
#include "raylib.h"
#include "Constants.h"

class Entity {
protected:
    bool active;
public:
    Entity();
    virtual ~Entity() = default;
    virtual void Update(float dt, int gameSpeed) = 0;
    virtual void Draw() = 0;
    bool IsActive() const;
    void SetActive(bool state);
};