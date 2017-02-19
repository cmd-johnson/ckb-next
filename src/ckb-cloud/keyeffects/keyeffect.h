#ifndef KEYEFFECT_H
#define KEYEFFECT_H

#include "color.h"

#include <QJsonObject>

class KeyEffect
{
public:
    KeyEffect();

    virtual bool advance(double deltaT);
    virtual Color getColor() const = 0;

    virtual QJsonObject toJson() const = 0;

protected:
    double phase;
};

#endif // KEYEFFECT_H
