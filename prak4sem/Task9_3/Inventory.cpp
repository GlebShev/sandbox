#include "Inventory.h"
#include <iostream>

Inventory::Inventory(int initialCapacity) : capacity(initialCapacity), count(0)
{
    items = new Item *[capacity];
}

Inventory::~Inventory()
{
    for (int i = 0; i < count; ++i)
    {
        delete items[i];
    }
    delete[] items;
}

void Inventory::resize(int newCapacity)
{
    Item **newItems = new Item *[newCapacity];
    for (int i = 0; i < count; ++i)
    {
        newItems[i] = items[i];
    }
    delete[] items;
    items = newItems;
    capacity = newCapacity;
}

void Inventory::addItem(Item *item)
{
    if (count == capacity)
    {
        resize(capacity * 2);
    }
    items[count++] = item;
}

void Inventory::removeItem(int index)
{
    if (index < 0 || index >= count)
    {
        std::cout << "Index out of range!" << std::endl;
        return;
    }
    delete items[index];
    for (int i = index; i < count - 1; ++i)
    {
        items[i] = items[i + 1];
    }
    --count;
}

int Inventory::size() const
{
    return count;
}

Item *Inventory::getItem(int index) const
{
    if (index < 0 || index >= count)
    {
        return nullptr;
    }
    return items[index];
}

// Реализация итератора
Inventory::Iterator::Iterator(Item **ptr) : current(ptr) {}

Inventory::Iterator &Inventory::Iterator::operator++()
{
    ++current;
    return *this;
}

bool Inventory::Iterator::operator!=(const Inventory::Iterator &other) const
{
    return current != other.current;
}

Item *Inventory::Iterator::operator*() const
{
    return *current;
}

Inventory::Iterator Inventory::begin()
{
    return Iterator(items);
}

Inventory::Iterator Inventory::end()
{
    return Iterator(items + count);
}

bool Inventory::contains(const std::string &itemName) const
{
    for (int i = 0; i < count; ++i)
    {
        if (items[i]->getName() == itemName)
            return true;
    }
    return false;
}
