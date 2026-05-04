#ifndef MAGICITEM_H
#define MAGICITEM_H

#include <string>

class MagicItem
{
protected:
    std::string magicEffect; // описание магического эффекта
public:
    MagicItem(const std::string &effect);
    virtual ~MagicItem();
    void castMagic();
    std::string getMagicEffect() const;
};

#endif
