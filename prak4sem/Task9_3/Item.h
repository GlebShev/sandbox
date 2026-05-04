#ifndef ITEM_H
#define ITEM_H

#include <string>
#include <iostream>

// Абстрактный базовый класс для всех предметов
class Item
{
protected:
    std::string name; // название предмета
    double weight;    // вес предмета
public:
    Item(const std::string &name, double weight) : name(name), weight(weight) {}
    virtual ~Item() {}

    // Чисто виртуальная функция, которую нужно переопределить в наследниках
    virtual void use() = 0;

    std::string getName() const { return name; }
    double getWeight() const { return weight; }
};

#endif