#include "../include/knight.h"
#include "../include/visitor.h"
#include <iostream>

Knight::Knight(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) {}

const std::string Knight::type_ = "Knight";

void Knight::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Knight::printInfo() const {
    std::cout << "Wandering Knight: " << getName()
              << " at coordinates (" << getX() << ", " << getY() << ")"
              << std::endl;
}