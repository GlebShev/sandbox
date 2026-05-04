#ifndef MAGICWEAPON_H
#define MAGICWEAPON_H

#include "Weapon.h"
#include "MagicItem.h"

class MagicWeapon : public Weapon, public MagicItem
{
public:
    MagicWeapon(const std::string &name, double weight, int damage, const std::string &magicEffect);
    virtual void use() override;
};

#endif
