#include "colorgradient.h"

#include <QJsonArray>

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

Color ColorGradient::getColor() const
{
    if (colorStops.empty()) {
        return Color(0, 0, 0, 0);
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

        const Color& a = first->color;
        const Color& b = second->color;

        return Color(
            a.redF() * inverseFactor + b.redF() * factor,
            a.greenF() * inverseFactor + b.greenF() * factor,
            a.blueF() * inverseFactor + b.blueF() * factor,
            a.alphaF() * inverseFactor + b.alphaF() * factor
        );
    }
}

QJsonObject ColorGradient::toJson() const
{
    QJsonArray colorStops;
    for (auto const& stop : this->colorStops) {
        QJsonObject colorStop = {
            { "position", stop.position },
            { "color", QString("#%1%2%3%4")
              .arg(stop.color.alpha(), 2, 16, QChar('0'))
              .arg(stop.color.red(), 2, 16, QChar('0'))
              .arg(stop.color.green(), 2, 16, QChar('0'))
              .arg(stop.color.blue(), 2, 16, QChar('0')) }
        };
        colorStops.append(colorStop);
    }

    QJsonObject json = {
        { "phase", phase },
        { "loop_count", (int)loopCount },
        { "duration", duration },
        { "current_color", QString("#%1%2%3%4")
          .arg(getColor().alpha(), 2, 16, QChar('0'))
          .arg(getColor().red(), 2, 16, QChar('0'))
          .arg(getColor().green(), 2, 16, QChar('0'))
          .arg(getColor().blue(), 2, 16, QChar('0'))
        },
        { "color_stops", colorStops }
    };



    return json;
}
