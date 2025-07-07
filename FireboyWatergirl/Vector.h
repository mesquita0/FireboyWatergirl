/**********************************************************************************
// Vector (Arquivo de Cabeçalho)
// 
// Criação:     18 Nov 2011
// Atualização: 20 Set 2023
// Compilador:  Visual C++ 2022
//
// Descrição:   Classe para representar um vetor
//
**********************************************************************************/

#ifndef _PROGJOGOS_VECTOR_H_
#define _PROGJOGOS_VECTOR_H_

// --------------------------------------------------------------------------------

#include <cmath>

constexpr double PI = 3.1415926535897932384626433832;

class Vector
{
private:
    float angle;                        // ângulo do vetor com o eixo x
    float magnitude;                    // magnitude do vetor

public:
    Vector();                           // construtor padrão
    Vector(float ang, float mag);       // construtor com ângulo e magnitude

    void Scale(float factor);           // amplia ou reduz vetor por um fator
    void ScaleTo(float scale);          // ajusta rotação para valor indicado
    void Rotate(float theta);           // rotaciona vetor por ângulo em graus
    void RotateTo(float value);         // ajusta rotação para valor indicado
    void Add(const Vector& v);          // adiciona vetor recebido por parâmetro
    
    float Angle() const;                // retorna ângulo do vetor
    void  Angle(float angle);
    float Magnitude() const;            // retorna magnitude do vetor
    float XComponent() const;           // retorna componente X do vetor
    void  XComponent(float x);
    float YComponent() const;           // retorna componente Y do vetor
    void  YComponent(float y);
    double Radians() const;              // retorna ângulo em radianos

    Vector operator*(double x) const;
}; 

// ---------------------------------------------------------------------------------
// Funções Membro Inline

inline void Vector::Scale(float factor)
{ ScaleTo(magnitude * factor); }

inline void Vector::RotateTo(float value)
{ angle = value; }

inline float Vector::Angle() const
{ return angle; }

inline float Vector::Magnitude() const
{ return magnitude; }

inline float Vector::XComponent() const
{ return (angle == 90 || angle == 270) ? 0 : magnitude * cos(Radians()); }

inline float Vector::YComponent() const
{ return (angle == 0 || angle == 180) ? 0 : magnitude * sin(Radians()); }

inline double Vector::Radians() const
{ return angle * PI / 180.0; }

inline Vector Vector::operator*(double x) const
{ return Vector(angle, magnitude * x); }

// ------------------------------------------------------------------------------

#endif