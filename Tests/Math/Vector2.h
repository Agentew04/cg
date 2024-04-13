#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <cmath>
#include <iostream>

class Vector2
{
public:
    float x, y;
    float magnitude;

    /// @brief Inicializa o vetor com 0,0
    Vector2();

    /// @brief Inicializa o vetor com x,y
    Vector2(float _x, float _y);

    /// @brief Inicializa o vetor com x,x
    Vector2(float _x);

    /// @brief Normaliza o vetor inplace
    void normalize();

    /// @brief Retorna um vetor normalizado
    Vector2 normalized();

    /// @brief troca x por y e y por x inplace
    void flip();

    /// @brief Troca x e y 
    Vector2 flipped();

    /// @brief Calcula a distancia entre dois pontos
    float distance(const Vector2& point) const;

    /// @brief Calcula o menor angulo entre dois vetores
    /// @param v O outro vetor a ser comparado
    /// @return O angulo em radianos
    float angle(const Vector2& v) const;

    bool isClockwise(const Vector2& v) const;

    // OPERADORES
    /// @brief Adiciona dois vetores
    Vector2 operator+(const Vector2 &v);
    Vector2 operator+=(const Vector2 &v);

    /// @brief Subtrai dois vetores
    Vector2 operator-(const Vector2 &v);
    Vector2 operator-=(const Vector2 &v);

    /// @brief Multiplica por um escalar
    Vector2 operator*(const float f);
    Vector2 operator*=(const float f);

    /// @brief Divide por um escalar
    Vector2 operator/(const float f);
    Vector2 operator/=(const float f);

    /// @brief Produtor escalar
    float operator*(const Vector2 &v) const;
    float dot(const Vector2 &v);

    /// @brief Multiplica componente a componente 2 vetores
    Vector2 multiply(const Vector2 &v);

    /// @brief Compara se dois vetores sao iguais
    bool operator==(const Vector2 &v);
    bool operator!=(const Vector2 &v);

    /// CONSTANTES
    static Vector2 zero() { return Vector2(0, 0); }
    static Vector2 one() { return Vector2(1, 1); }
    static Vector2 left() { return Vector2(-1, 0); }
    static Vector2 right() { return Vector2(1, 0); }
    static Vector2 up() { return Vector2(0, -1); }
    static Vector2 down() { return Vector2(0, 1); }

    friend std::ostream &operator<<(std::ostream &os, const Vector2 &obj);
};

#endif
