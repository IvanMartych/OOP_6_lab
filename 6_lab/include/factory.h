#pragma once
#include <memory>
#include <string>
#include "npc.h"

// фабрика для создания NPC
class NpcFactory {
public:
    // создание NPC по типу
    static std::unique_ptr<Npc> createNpc(
        const std::string& type,
        const std::string& name,
        int x, 
        int y
    );
    
    // создание NPC из строки файла
    static std::unique_ptr<Npc> createFromString(const std::string& line);
};