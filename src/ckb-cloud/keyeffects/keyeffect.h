#ifndef KEYEFFECT_H
#define KEYEFFECT_H

#include "color.h"

class KeyEffect
{
public:
    KeyEffect();

    virtual bool advance(double deltaT);
    virtual Color getColor() const = 0;

protected:
    double phase;
};

#endif // KEYEFFECT_H
