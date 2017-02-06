#ifndef KEYEFFECT_H
#define KEYEFFECT_H

#include <QColor>

class KeyEffect
{
public:
    KeyEffect();

    virtual bool advance(double deltaT);
    virtual QColor getColor() const = 0;

protected:
    double phase;
};

#endif // KEYEFFECT_H
