#include "../include/arena.h"
#include "../include/factory.h"
#include "../include/combat_visitor.h"
#include <iostream>
#include <memory>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

// конструктор с валидацией границ
Arena::Arena(int width, int height) {
    if (width > MAX_WIDTH || height > MAX_HEIGHT) {
        throw std::out_of_range("Arena dimensions exceed maximum limits.");
    }
    this->width_ = width;
    this->height_ = height;
}

// добавление NPC с валидацией
void Arena::addNpc(std::unique_ptr<Npc> npc) {
    const std::string name = npc->getName();

    // проверка границ
    if (npc->getX() < 0 || npc->getX() > width_ ||
        npc->getY() < 0 || npc->getY() > height_) {
        throw std::out_of_range("NPC coordinates out of bounds.");
    }

    // проверка на дубликаты имен
    if (npcs_.find(name) != npcs_.end()) {
        throw std::invalid_argument("NPC with this name already exists.");
    }
    
    npcs_[name] = std::move(npc);
}

void Arena::createAndAddNpc(const std::string& type, 
                            const std::string& name, 
                            int x, int y) {
    auto npc = NpcFactory::createNpc(type, name, x, y);
    npcs_[name] = std::move(npc);
}

// вывод всех NPC, находящихся на арене
void Arena::printAllNpcs() const {
    for (const auto& pair : npcs_) {
        std::cout << *(pair.second) << std::endl;
    }
}

// возврат текущего количества NPC
size_t Arena::getNpcCount() const {
    return npcs_.size();
}

// сохранение NPC в файл
void Arena::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    for (const auto& [name, npc] : npcs_) {
        file << npc->getType() << " "
             << npc->getName() << " "
             << npc->getX() << " "
             << npc->getY() << std::endl;
    }
}

// загрузка NPC из файла
void Arena::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        auto npc = NpcFactory::createFromString(line);
        addNpc(std::move(npc));
    }
}

// очистка арены
void Arena::clear() {
    npcs_.clear();
}

// управление наблюдателями
void Arena::addObserver(std::shared_ptr<Observer> observer) {
    observers_.push_back(observer);
}

void Arena::removeObserver(std::shared_ptr<Observer> observer) {
    observers_.erase(
        std::remove(observers_.begin(), observers_.end(), observer),
        observers_.end()
    );
}

// уведомление всех наблюдателей
void Arena::notifyObservers(const std::string& event) {
    for (auto& observer : observers_) {
        observer->notify(event);
    }
}

// боевая система: проверка всех пар NPC в пределах дальности
void Arena::startBattle(double range) {
    CombatVisitor visitor;
    std::vector<std::string> toRemove;

    // проверка каждой пары NPC
    for (auto& [name1, npc1] : npcs_) {
        for (auto& [name2, npc2] : npcs_) {
            if (name1 == name2) continue;
            if (name1 > name2) continue; // избегаем дублирования пар

            // проверка, находятся ли в пределах дальности боя
            if (npc1->distanceTo(*npc2) > range) continue;
            
            // проверка боя в обоих направлениях
            bool npc1KillsNpc2 = visitor.canKill(npc1.get(), npc2.get());
            bool npc2KillsNpc1 = visitor.canKill(npc2.get(), npc1.get());
            
            if (npc1KillsNpc2 && npc2KillsNpc1) {
                // взаимное убийство
                std::string event = npc1->getName() + " (" + npc1->getType() + 
                                   ") and " + npc2->getName() + " (" + npc2->getType() + 
                                   ") killed each other";
                notifyObservers(event);
                toRemove.push_back(name1);
                toRemove.push_back(name2);
            } else if (npc1KillsNpc2) {
                // только npc1 убивает npc2
                std::string event = npc1->getName() + " (" + npc1->getType() + 
                                   ") killed " + npc2->getName() + " (" + npc2->getType() + ")";
                notifyObservers(event);
                toRemove.push_back(name2);
            } else if (npc2KillsNpc1) {
                // только npc2 убивает npc1
                std::string event = npc2->getName() + " (" + npc2->getType() + 
                                   ") killed " + npc1->getName() + " (" + npc1->getType() + ")";
                notifyObservers(event);
                toRemove.push_back(name1);
            }
        }
    }
    
    // удаление мёртвых NPC (сначала устраняем дубликаты)
    std::sort(toRemove.begin(), toRemove.end());
    toRemove.erase(std::unique(toRemove.begin(), toRemove.end()), toRemove.end());
    
    for (const auto& name : toRemove) {
        npcs_.erase(name);
    }
}