#include "../include/squirrel.h"
#include "../include/visitor.h"
#include <iostream>

Squirrel::Squirrel(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) {}

const std::string Squirrel::type_ = "Squirrel";

void Squirrel::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Squirrel::printInfo() const {
    std::cout << "Squirrel: " << getName()
              << " positioned at (" << getX() << ", " << getY() << ")"
              << std::endl;
} 