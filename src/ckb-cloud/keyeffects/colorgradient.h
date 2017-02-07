#ifndef COLORGRADIENT_H
#define COLORGRADIENT_H

#include "keyeffect.h"

#include <QList>

class ColorGradient : public KeyEffect
{
public:
    struct ColorStop {
        double position;
        QColor color;
    };

    ColorGradient(QList<ColorStop> colorStops, double duration = 1.0,
                  uint loopCount = 1);

    virtual bool advance(double deltaT) override;
    virtual QColor getColor() const override;

private:
    QList<ColorStop> colorStops;
    double duration;
    uint loopCount;
};

#endif // COLORGRADIENT_H
