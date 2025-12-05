#pragma once
#include <string>
#include "npc.h"
#include <map>
#include <memory>
#include "observer.h"
#include <vector>

#define MAX_WIDTH 500
#define MAX_HEIGHT 500

// арена для сражений npc
class Arena {
private:
    int width_, height_;
    std::map<std::string, std::unique_ptr<Npc>> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;

public:
    Arena(int width = MAX_WIDTH, int height = MAX_HEIGHT);

    // работа с файлами
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename) const;

    // управление наблюдателями
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);

    // боевая механика
    void startBattle(double range);

    // добавление npc
    void addNpc(std::unique_ptr<Npc> npc);
    void createAndAddNpc(const std::string& type, 
                         const std::string& name, 
                         int x, int y);

    // информация и очистка
    void printAllNpcs() const;
    size_t getNpcCount() const;
    void clear();

    void notifyObservers(const std::string& event);
};