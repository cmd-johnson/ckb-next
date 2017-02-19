#ifndef FIXEDCOLOR_H
#define FIXEDCOLOR_H

#include "keyeffect.h"

class FixedColor : public KeyEffect
{
public:
    FixedColor(const Color& color);

    virtual bool advance(double deltaT) override;
    virtual Color getColor() const override;

    virtual QJsonObject toJson() const override;

private:
    Color color;
};

#endif // FIXEDCOLOR_H
