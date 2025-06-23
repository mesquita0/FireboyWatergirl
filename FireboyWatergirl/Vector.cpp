/**********************************************************************************
// Vector (Codigo Fonte)
// 
// Cria��o:     18 Nov 2011
// Atualiza��o: 20 Set 2023
// Compilador:  Visual C++ 2022
//
// Descri��o:   Classe para representar um vetor
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

void Vector::Scale(float factor)
{
    magnitude *= factor;
}

// ------------------------------------------------------------------------------

void Vector::Rotate(float theta)
{
    angle -= theta;

    // mant�m o �ngulo entre 0 e 359 graus
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

    // ajusta o �ngulo de acordo com o quadrante do vetor resultante
    if (rx > 0)
    {
        // 1o Quadrante
        if (ry >= 0)
        {
            // acha o �ngulo em radianos
            angle = atan(ry / rx);
            // converte de radianos para graus
            angle = float((180.0 * angle) / PI);
        }
        // 4o Quadrante
        else // (ry < 0)
        {
            // acha o �ngulo em radianos
            angle = atan(ry / rx);
            // converte de radianos para graus
            angle = float((180.0 * angle) / PI) + 360.0f;
        }
    }
    // 2o e 3o Quadrante
    else if (rx < 0)
    {
        // acha o �ngulo em radianos
        angle = atan(ry / rx);
        // converte de radianos para graus
        angle = float((180.0 * angle) / PI) + 180.0f;
    }
    else // (rx == 0)
    {
        if (ry > 0)
            angle = 90.0f;
        else if (ry < 0)
            angle = 270.0f;
        else // (ry == 0)
            angle = v.angle;
    }
}

void Vector::XComponent(float x)
{
    if (magnitude == 0 || angle == 0 || angle == 180) {
        magnitude = x;
        angle = 0;
    }
    else {
        float old_angle = Radians();
        angle = atan(YComponent()/x) * (180 / PI);
        magnitude *= sin(old_angle) / sin(Radians());
    }
}

void Vector::YComponent(float y)
{
    if (magnitude == 0 || angle == 90 || angle == 270) {
        magnitude = y;
        angle = 90;
    }
    else {
        float old_angle = Radians();
        angle = atan(y / XComponent()) * (180 / PI);
        magnitude *= cos(old_angle) / cos(Radians());
    }
}

// ------------------------------------------------------------------------------