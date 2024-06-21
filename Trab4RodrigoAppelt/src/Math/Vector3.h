#ifndef __VECTOR_3_H__
#define __VECTOR_3_H__

#include <iostream>

#include "Vector2.h"

class Vector3
{
public:
    float x, y, z;
    float magnitude;

    Vector3();

    Vector3(float _x);

    Vector3(float _x, float _y);

    Vector3(float _x, float _y, float _z);

    static Vector3 fromHex(uint32_t hex);

    void normalize();

    Vector3 normalized();

    Vector2 toVector2() const;

    /// @brief Calcula a distancia entre dois pontos
    float distance(Vector3 point);

    // OPERADORES

    /// @brief Adiciona dois vetore
    Vector3 operator + (const Vector3& v) const;
    Vector3& operator += (const Vector3& v);

    /// @brief Inverte o sinal do vetor
    Vector3 operator - () const;
    Vector3 inverted() const;
    void invert();

    /// @brief Subtrai dois vetores
    Vector3 operator - (const Vector3& v) const;
    Vector3& operator -= (const Vector3& v) ;

    /// @brief Multiplica por um escalar
    Vector3 operator * (float value) const;
    Vector3& operator *= (float value);

    /// @brief Divide por um escalar
    Vector3 operator / (float value) const;
    Vector3& operator /= (float value);

    /// @brief Produtor escalar
    float operator * (const Vector3& v) const;
    float dot(const Vector3& v);

    /// @brief Produtor vetorial
    Vector3 operator ^ (const Vector3& v);
    Vector3& operator ^= (const Vector3& v);
    Vector3 cross(const Vector3& v);

    /// @brief Multiplica componente a componente 2 vetores
    Vector3 multiply(const Vector3& v);

    /// @brief Compara se dois vetores sao iguais
    bool operator == (const Vector3& v);
    bool operator != (const Vector3& v);

    // CONSTANTES
    static Vector3 zero() { return Vector3(0, 0, 0); }
    static Vector3 one() { return Vector3(1, 1, 1); }
    static Vector3 left() { return Vector3(-1, 0, 0); }
    static Vector3 right() { return Vector3(1, 0, 0); }
    static Vector3 up() { return Vector3(0, -1, 0); }
    static Vector3 down() { return Vector3(0, 1, 0); }
    static Vector3 forward() { return Vector3(0, 0, -1); }
    static Vector3 back() { return Vector3(0, 0, 1); }

    friend std::ostream& operator<<(std::ostream& os, const Vector3& obj);
};

#endif
