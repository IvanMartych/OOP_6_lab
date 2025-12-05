#include "../include/npc.h"
#include <cmath>
#include <ostream>
#include <iostream>

// реализация конструктора
Npc::Npc(int x, int y, const std::string& type, const std::string& name)
    : x_(x), y_(y), type_(type), name_(name) {}

// реализация геттеров для координат и свойств
int Npc::getX() const { 
    return x_; 
}

int Npc::getY() const { 
    return y_; 
}

std::string Npc::getType() const { 
    return type_; 
}

std::string Npc::getName() const { 
    return name_; 
}

// вычисление расстояния по теореме пифагора
double Npc::distanceTo(const Npc& other) const {
    int dx = x_ - other.x_;
    int dy = y_ - other.y_;
    return std::sqrt(dx * dx + dy * dy);
}

void Npc::printInfo() const {
    std::cout << *this << std::endl;
}

// оператор вывода в поток
std::ostream& operator<<(std::ostream& os, const Npc& npc) {
    os << "NPC [" << npc.type_ << "] " << npc.name_
       << " @ (" << npc.x_ << ", " << npc.y_ << ")";
    return os;
}