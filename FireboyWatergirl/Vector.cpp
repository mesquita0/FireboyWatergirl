/**********************************************************************************
// Vector (Codigo Fonte)
// 
// Criação:     18 Nov 2011
// Atualização: 20 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe para representar um vetor
//
**********************************************************************************/

#include "Vector.h"

// ------------------------------------------------------------------------------

Vector::Vector()
{
    angle     = 0;
    magnitude = 0;
}

// ------------------------------------------------------------------------------

Vector::Vector(float ang, float mag)
{
    angle     = ang;
    magnitude = mag;
}

// ------------------------------------------------------------------------------

void Vector::ScaleTo(float scale)
{
    magnitude = abs(scale);
    if (scale < 0) Rotate(180);
}

// ------------------------------------------------------------------------------

void Vector::Rotate(float theta)
{
    angle -= theta;

    // mantém o ângulo entre 0 e 359 graus
    while (angle < 0 || angle >= 360)
        angle += (angle >= 360 ? -360.0f : 360.0f);
}

// ------------------------------------------------------------------------------

void Vector::Add(const Vector& v)
{
    float rx, ry;

    rx = XComponent() + v.XComponent();
    ry = YComponent() + v.YComponent();

    magnitude = sqrt(pow(rx, 2.0f) + pow(ry, 2.0f));

    if (rx == 0)
    {
        if (ry > 0)
            angle = 90.0f;
        else if (ry < 0)
            angle = 270.0f;
        else // (ry == 0)
            angle = v.angle;
    }
    else {
        // acha o ângulo em radianos
        angle = atan2(ry, rx);

        // converte de radianos para graus
        Angle(float((180.0 * angle) / PI));
    }
}

void Vector::Angle(float angle) {
    this->angle = fmod(angle, 360);
    if (this->angle < 0) {
        this->angle += 360;
    }
}

void Vector::XComponent(float x)
{
    if (magnitude == 0 || angle == 0 || angle == 180) {
        magnitude = abs(x);
        angle = (x > 0) ? 0 : 180;
    }
    else {
        float old_angle = Radians();
        float old_y = YComponent();
        magnitude = sqrt(pow(x, 2) + pow(YComponent(), 2));
        Angle(atan2(old_y, x) * (180 / PI));
    }
}

void Vector::YComponent(float y)
{
    if (magnitude == 0 || angle == 90 || angle == 270) {
        magnitude = abs(y);
        angle = (y > 0) ? 90 : 270;
    }
    else {
        float old_angle = Radians();
        float old_x = XComponent();
        magnitude = sqrt(pow(XComponent(), 2) + pow(y, 2));
        Angle(atan2(y, old_x) * (180 / PI));
    }
}

// ------------------------------------------------------------------------------