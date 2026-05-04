#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include <string>

class Inventory
{
private:
    Item **items; // динамический массив указателей на Item
    int capacity; // текущая вместимость массива
    int count;    // количество добавленных элементов
    void resize(int newCapacity);

public:
    Inventory(int initialCapacity = 10);
    ~Inventory();
    void addItem(Item *item);
    void removeItem(int index);
    int size() const;
    Item *getItem(int index) const;

    // Вложенный класс итератора
    class Iterator
    {
    private:
        Item **current;

    public:
        Iterator(Item **ptr);
        Iterator &operator++();
        bool operator!=(const Iterator &other) const;
        Item *operator*() const;
    };

    Iterator begin();
    Iterator end();
    bool contains(const std::string &itemName) const;
};

#endif
