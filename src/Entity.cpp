#include "../include/Entity.h"

Entity::Entity() : active(true) {}

bool Entity::IsActive() const { return active; }
void Entity::SetActive(bool state) { active = state; }