#ifndef COLOR_H
#define COLOR_H

#include <QString>

class Color {
public:
    Color();
    Color(int r, int g, int b, int a);
    Color(double r, double g, double b, double a);
    Color(const QString& rgba);

    int red() const;
    double redF() const;
    void setRed(int r);
    void setRed(double r);

    int green() const;
    double greenF() const;
    void setGreen(int g);
    void setGreen(double g);

    int blue() const;
    double blueF() const;
    void setBlue(int b);
    void setBlue(double b);

    int alpha() const;
    double alphaF() const;
    void setAlpha(int a);
    void setAlpha(double a);

    bool isValid() const;

    Color blend(const Color& other) const;

    QString toString() const;

private:
    uint32_t color;
    bool valid;

    void setValid(bool valid);
};

#endif // COLOR_H
