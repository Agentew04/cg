#include "Vector2.h"

Vector2::Vector2()
{
    x = y = 0;
}

Vector2::Vector2(float _x)
{
    x = y = _x;
    magnitude = sqrt(x*x + y*y);
}

Vector2::Vector2(float _x, float _y)
{
    x = _x;
    y = _y;
    magnitude = sqrt(x*x + y*y);
}

void Vector2::normalize()
{
    float norm = (float)sqrt(x*x + y*y);

    if(norm==0.0)
    {
        //printf("\n\nNormalize::Divisao por zero");
        x = 0;
        y = 0;
        return;
    }
    x /= norm;
    y /= norm;
    magnitude = 1.0;
}

Vector2 Vector2::normalized()
{
    Vector2 v(x, y);
    v.normalize();
    return v;
}

Vector2 Vector2::flipped()
{
    return Vector2(y, x);
}

void Vector2::flip()
{
    float temp = x;
    x = y;
    y = temp;
}

float Vector2::distance(Vector2 point)
{
    return std::sqrt( ((point.x - x)*(point.x - x)) + ((point.y - y)*(point.y - y)));
}

Vector2 Vector2::operator+(const Vector2 &v)
{
    return Vector2(x + v.x, y + v.y);
}

Vector2 Vector2::operator+=(const Vector2 &v)
{
    x += v.x;
    y += v.y;
    return *this;
}

Vector2 Vector2::operator-(const Vector2 &v)
{
    return Vector2(x - v.x, y - v.y);
}

Vector2 Vector2::operator-=(const Vector2 &v)
{
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2 Vector2::operator*(const float f)
{
    return Vector2(x * f, y * f);
}

Vector2 Vector2::operator*=(const float f)
{
    x *= f;
    y *= f;
    return *this;
}

Vector2 Vector2::operator/(const float f)
{
    return Vector2(x / f, y / f);
}

Vector2 Vector2::operator/=(const float f)
{
    x /= f;
    y /= f;
    return *this;
}

float Vector2::operator*(const Vector2 &v)
{
    return x * v.x + y * v.y;
}

float Vector2::dot(const Vector2 &v)
{
    return x * v.x + y * v.y;
}

Vector2 Vector2::multiply(const Vector2 &v)
{
    return Vector2(x * v.x, y * v.y);
}

bool Vector2::operator==(const Vector2 &v)
{
    float epsilon = 0.00001;
    return fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon;
}

bool Vector2::operator!=(const Vector2 &v)
{
    return !(*this == v);
}

std::ostream &operator<<(std::ostream &os, const Vector2 &obj)
{
    os << "(" << obj.x << ", " << obj.y << ")";
    return os;
}