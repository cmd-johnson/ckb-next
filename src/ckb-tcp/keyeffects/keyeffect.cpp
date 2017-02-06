#include "keyeffect.h"

KeyEffect::KeyEffect() : phase(0)
{
}

bool KeyEffect::advance(double deltaT)
{
    phase += deltaT;
    return phase > 1.0;
}
