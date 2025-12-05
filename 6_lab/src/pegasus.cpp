#include "../include/pegasus.h"
#include "../include/visitor.h"
#include <iostream>

Pegasus::Pegasus(int x, int y, const std::string& name)
    : Npc(x, y, type_, name) {}

const std::string Pegasus::type_ = "Pegasus";


void Pegasus::accept(Visitor& visitor) {
    visitor.visit(*this);
}

void Pegasus::printInfo() const {
    std::cout << "Pegasus: " << getName()
              << " flying at (" << getX() << ", " << getY() << ")"
              << std::endl;
}