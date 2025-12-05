#include <memory>
#include <iostream>
#include "../include/factory.h"
#include "../include/knight.h"
#include "../include/pegasus.h"
#include "../include/squirrel.h"
#include <sstream>
#include <stdexcept>

std::unique_ptr<Npc> NpcFactory::createNpc(
    const std::string& type,
    const std::string& name,
    int x,
    int y)
    {
        if (type == "Knight") {
            return std::make_unique<Knight>(x, y, name);
        } else if (type == "Squirrel") {
            return std::make_unique<Squirrel>(x, y, name);
        } else if (type == "Pegasus") {
            return std::make_unique<Pegasus>(x, y, name);
        } else {
            throw std::invalid_argument("Unknown NPC type: " + type);
        }   
    }

std::unique_ptr<Npc> NpcFactory::createFromString(const std::string& line) {
    std::istringstream iss(line);
    std::string type, name;
    int x, y;

    iss >> type >> name >> x >> y;
    
    if (iss.fail()) {
        throw std::runtime_error("Failed to read line: " + line);
    }

    return createNpc(type, name, x ,y);
}