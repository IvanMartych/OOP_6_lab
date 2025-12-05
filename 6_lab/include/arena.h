#pragma once
#include <string>
#include "npc.h"
#include <map>
#include <memory>
#include "observer.h"
#include <vector>

#define MAX_WIDTH 500
#define MAX_HEIGHT 500

// главная игровая арена для управления NPC и боем
class Arena {
public:
    Arena(int width = MAX_WIDTH, int height = MAX_HEIGHT);

    // добавление существующего NPC на арену
    void addNpc(std::unique_ptr<Npc> npc);

    // создание и добавление нового NPC
    void createAndAddNpc(const std::string& type, 
                         const std::string& name, 
                         int x, int y);

    // отображение всех NPC на арене
    void printAllNpcs() const;

    // получение общего количества NPC
    size_t getNpcCount() const;

    // методы паттерна наблюдатель
    void addObserver(std::shared_ptr<Observer> observer);
    void removeObserver(std::shared_ptr<Observer> observer);

    // боевая система
    void startBattle(double range);

    // операции ввода/вывода в файл
    void saveToFile(const std::string& filename) const;
    void loadFromFile(const std::string& filename);

    // очистка всех NPC
    void clear();
    
private:
    int width_;
    int height_;
    std::map<std::string, std::unique_ptr<Npc>> npcs_;
    std::vector<std::shared_ptr<Observer>> observers_;

    void notifyObservers(const std::string& event);
};