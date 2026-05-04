#include "Weapon.h"
#include <iostream>

Weapon::Weapon(const std::string &name, double weight, int damage)
    : Item(name, weight), damage(damage) {}

void Weapon::use()
{
    std::cout << "Attacking with " << name << " causing " << damage << " damage." << std::endl;
}

int Weapon::getDamage() const
{
    return damage;
}
