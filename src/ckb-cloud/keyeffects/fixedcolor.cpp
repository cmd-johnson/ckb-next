#include "fixedcolor.h"

FixedColor::FixedColor(const Color& color) : color(color)
{
}

bool FixedColor::advance(double deltaT)
{
    // We don't actually change the phase of the animation here,
    // since the color doesn't change over time and just stays as
    // it is, until the effect is either removed or replaced.
    Q_UNUSED(deltaT);
    return true;
}

Color FixedColor::getColor() const
{
    return color;
}
