#include "Potion.h"
#include <iostream>

Potion::Potion(const std::string &name, double weight, int healing)
    : Item(name, weight), healing(healing) {}

void Potion::use()
{
    std::cout << "Drinking potion " << name << " to heal " << healing << " HP." << std::endl;
}

int Potion::getHealing() const
{
    return healing;
}
