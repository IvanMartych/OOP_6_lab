#pragma once
#include <string>
#include "npc.h"

// класс пегаса - мирное существо, никого не атакует
class Pegasus : public Npc {
public:
    Pegasus(int x, int y, const std::string& name);

    void accept(Visitor& visitor) override;
    void printInfo() const override;

private:
    static const std::string type_;
};
