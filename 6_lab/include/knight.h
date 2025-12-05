#pragma once
#include <string>
#include "npc.h"

// класс странствующего рыцаря - атакует белок
class Knight : public Npc {
public:
    Knight(int x, int y, const std::string& name);

    void accept(Visitor& visitor) override;
    void printInfo() const override;

private:
    static const std::string type_;
};