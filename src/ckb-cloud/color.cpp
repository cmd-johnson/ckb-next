#include "color.h"

#include <QRegExp>

namespace {
    uint32_t rgbaToUint32(int r, int g, int b, int a)
    {
        return (r & 0xff) << 24 |
               (g & 0xff) << 16 |
               (b & 0xff) << 8  |
               (a & 0xff);
    }

    QRegExp& colorRegex()
    {
        static QRegExp colorRegex("^#([0-9a-f]{8})$");
        colorRegex.setCaseSensitivity(Qt::CaseInsensitive);
        return colorRegex;
    }
}

Color::Color() : color(0), valid(false) { }

Color::Color(int r, int g, int b, int a) : color(0), valid(false)
{
    if (((r | g | b | a) & ~0xff) != 0) { return; }
    color = ((uint) r << 24) | ((uint) g << 16) | ((uint) b << 8) | (uint) a;
    setValid(true);
}

Color::Color(double r, double g, double b, double a) : color(0), valid(false)
{
    if (r < 0.0 || r > 1.0) { return; }
    if (g < 0.0 || g > 1.0) { return; }
    if (b < 0.0 || b > 1.0) { return; }
    if (a < 0.0 || a > 1.0) { return; }

    color = rgbaToUint32(
        (uchar)(r * 255), (uchar)(g * 255), (uchar)(b * 255), (uchar)(a * 255)
    );
    setValid(true);
}

Color::Color(const QString &rgba) : color(0), valid(false)
{
    QRegExp& regex = colorRegex();

    if (regex.indexIn(rgba) < 0) { return; }

    bool status;
    uint color = regex.cap(1).toUInt(&status, 16);
    if (!status) { return; }

    this->color = (uint32_t)color;
    setValid(true);
}

int Color::red() const
{
    return (color >> 24) & 0xff;
}

double Color::redF() const
{
    return red() / 255.0;
}

void Color::setRed(int r)
{
    setValid((r & ~0xff) == 0);
    if (!isValid()) {
        return;
    }
    color = (color & ~(0xff << 24)) | ((uint32_t) r) << 24;
}

void Color::setRed(double r)
{
    setValid(r >= 0.0 && r <= 1.0);
    if (!isValid()) {
        return;
    }
    setRed(r * 255);
}

int Color::green() const
{
    return (color >> 16) & 0xff;
}

double Color::greenF() const
{
    return green() / 255.0;
}

void Color::setGreen(int g)
{
    setValid((g & ~0xff) == 0);
    if (!isValid()) {
        return;
    }
    color = (color & ~(0xff << 16)) | ((uint32_t) g) << 16;
}

void Color::setGreen(double g)
{
    setValid(g >= 0.0 && g <= 1.0);
    if (!isValid()) {
        return;
    }
    setGreen(g * 255);
}

int Color::blue() const
{
    return (color >> 8) & 0xff;
}

double Color::blueF() const
{
    return blue() / 255.0;
}

void Color::setBlue(int b)
{
    setValid((b & ~0xff) == 0);
    if (!isValid()) {
        return;
    }
    color = (color & ~(0xff << 8)) | ((uint32_t) b) << 8;
}

void Color::setBlue(double b)
{
    setValid(b >= 0.0 && b <= 1.0);
    if (!isValid()) {
        return;
    }
    setGreen(b * 255);
}

int Color::alpha() const
{
    return color & 0xff;
}

double Color::alphaF() const
{
    return alpha() / 255.0;
}

void Color::setAlpha(int a)
{
    setValid((a & ~0xff) == 0);
    if (!isValid()) {
        return;
    }
    color = (color & ~0xff) | (uint32_t) a;
}

void Color::setAlpha(double a)
{
    setValid(a >= 0.0 && a <= 1.0);
    if (!isValid()) {
        return;
    }
    setAlpha(a * 255);
}

bool Color::isValid() const
{
    return this->valid;
}

Color Color::blend(const Color &other) const
{
    double a = other.alphaF() * (1 - alphaF()) + alphaF();
    return Color(
        redF()   * alphaF() / a + other.redF()   * other.alphaF() * (1 - alphaF()) / a,
        greenF() * alphaF() / a + other.greenF() * other.alphaF() * (1 - alphaF()) / a,
        blueF()  * alphaF() / a + other.blueF()  * other.alphaF() * (1 - alphaF()) / a,
        a
    );
}

QString Color::toString() const
{
    if (!isValid()) {
        return QString("Invalid Color");
    }
    return QString("#%1%2%3%4")
            .arg(red(), 2, 16, QChar('0'))
            .arg(green(), 2, 16, QChar('0'))
            .arg(blue(), 2, 16, QChar('0'))
            .arg(alpha(), 2, 16, QChar('0'));
}

void Color::setValid(bool valid)
{
    this->valid = valid;
    if (!valid) {
        this->color = 0;
    }
}
