#include "colorgradient.h"

#include <math.h>

ColorGradient::ColorGradient(QList<ColorGradient::ColorStop> colorStops,
                             double duration, uint loopCount)
    : colorStops(colorStops), duration(duration), loopCount(loopCount)
{
}

bool ColorGradient::advance(double deltaT)
{
    phase += deltaT / duration;
    if (phase > 1.0) {
        if (loopCount == 1) {
            return false;
        }

        phase = fmod(phase, 1.0);
        if (loopCount > 1) {
            loopCount--;
        }
    }
    return true;
}

QColor ColorGradient::getColor() const
{
    if (colorStops.empty()) {
        return Qt::transparent;
    }

    double phase = this->phase;
    auto second = std::find_if(colorStops.begin(), colorStops.end(),
                                    [phase](const ColorStop& stop) {
        return stop.position > phase;
    });

    auto first = std::prev(second);

    if (second == colorStops.end()) {
        return first->color;
    } else if (phase >= second->position) {
        return second->color;
    } else {
        double factor = (phase - first->position) / (second->position - first->position);
        double inverseFactor = 1 - factor;

        const QColor& a = first->color;
        const QColor& b = second->color;

        return QColor::fromRgbF(
            a.redF() * inverseFactor + b.redF() * factor,
            a.greenF() * inverseFactor + b.greenF() * factor,
            a.blueF() * inverseFactor + b.blueF() * factor,
            a.alphaF() * inverseFactor + b.alphaF() * factor
        );
    }
}
