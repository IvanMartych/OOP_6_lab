#pragma once
#include <string>
#include <memory>

// предварительное объявление
class Visitor;

// базовый класс для всех NPC в игре
class Npc {
public:
    Npc(int x, int y, const std::string& type, const std::string& name);
    virtual ~Npc() = default;

    // геттеры для свойств NPC
    int getX() const;
    int getY() const;
    std::string getType() const;
    std::string getName() const;

    // вычисление евклидова расстояния до другого NPC
    double distanceTo(const Npc& other) const;

    // реализация паттерна посетитель
    virtual void accept(Visitor& visitor) = 0;

    // отображение информации об NPC
    virtual void printInfo() const;

    // перегрузка оператора вывода
    friend std::ostream& operator<<(std::ostream& os, const Npc& npc);

private:
    int x_;
    int y_;
    std::string type_;
    std::string name_;
};