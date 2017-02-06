#ifndef FIXEDCOLOR_H
#define FIXEDCOLOR_H

#include "keyeffect.h"

class FixedColor : public KeyEffect
{
public:
    FixedColor(const QColor& color);

    virtual bool advance(double deltaT) override;
    virtual const QColor& getColor() const override;

private:
    QColor color;
};

#endif // FIXEDCOLOR_H
