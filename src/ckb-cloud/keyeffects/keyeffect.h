#ifndef KEYEFFECT_H
#define KEYEFFECT_H

#include "color.h"

#include <QJsonObject>
#include <QUuid>

class KeyEffect
{
public:
    KeyEffect();

    virtual bool advance(double deltaT);
    virtual Color getColor() const = 0;

    virtual QJsonObject toJson() const = 0;

    const QUuid& getUuid() const;

protected:
    double phase;
    const QUuid uuid;
};

#endif // KEYEFFECT_H
