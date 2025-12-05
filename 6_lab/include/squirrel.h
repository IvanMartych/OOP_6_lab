#pragma once
#include <string>
#include "npc.h"

// класс белки - атакует пегасов
class Squirrel : public Npc {
public:
    Squirrel(int x, int y, const std::string& name);

    void accept(Visitor& visitor) override;
    void printInfo() const override;

private:
    static const std::string type_;
};