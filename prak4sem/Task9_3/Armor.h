#ifndef ARMOR_H
#define ARMOR_H

#include "Item.h"

class Armor : public Item
{
private:
    int defense; // уровень защиты
public:
    Armor(const std::string &name, double weight, int defense);
    virtual void use() override;
    int getDefense() const;
};

#endif
