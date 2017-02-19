#include "keyeffect.h"

KeyEffect::KeyEffect() : phase(0), uuid(QUuid::createUuid())
{
}

bool KeyEffect::advance(double deltaT)
{
    phase += deltaT;
    return phase > 1.0;
}

const QUuid& KeyEffect::getUuid() const
{
    return uuid;
}
