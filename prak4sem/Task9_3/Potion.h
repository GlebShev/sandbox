#ifndef POTION_H
#define POTION_H

#include "Item.h"

class Potion : public Item
{
private:
    int healing; // величина лечения
public:
    Potion(const std::string &name, double weight, int healing);
    virtual void use() override;
    int getHealing() const;
};

#endif
