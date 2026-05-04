#include "Armor.h"
#include <iostream>

Armor::Armor(const std::string &name, double weight, int defense)
    : Item(name, weight), defense(defense) {}

void Armor::use()
{
    std::cout << "Equipping armor " << name << " increasing defense by " << defense << "." << std::endl;
}

int Armor::getDefense() const
{
    return defense;
}
