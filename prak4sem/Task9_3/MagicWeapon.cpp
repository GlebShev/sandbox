#include "MagicWeapon.h"
#include <iostream>

MagicWeapon::MagicWeapon(const std::string &name, double weight, int damage, const std::string &magicEffect)
    : Weapon(name, weight, damage), MagicItem(magicEffect) {}

void MagicWeapon::use()
{
    std::cout << "Attacking with magic weapon " << name << " causing " << damage
              << " damage and applying effect: " << magicEffect << "." << std::endl;
}
