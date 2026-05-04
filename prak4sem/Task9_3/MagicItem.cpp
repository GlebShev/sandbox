#include "MagicItem.h"
#include <iostream>

MagicItem::MagicItem(const std::string &effect)
    : magicEffect(effect) {}

MagicItem::~MagicItem() {}

void MagicItem::castMagic()
{
    std::cout << "Casting magic: " << magicEffect << std::endl;
}

std::string MagicItem::getMagicEffect() const
{
    return magicEffect;
}
