#include <cmath>

#include "Vector3.h"

Vector3::Vector3()
{
    x = y = z = 0;
}

Vector3::Vector3(float _x)
{
    x = y = z = _x;
    magnitude = sqrt(x*x + y*y + z*z);
}

Vector3::Vector3(float _x, float _y)
{
    x = _x;
    y = _y;
    z = 0;
    magnitude = sqrt(x*x + y*y + z*z);
}

Vector3::Vector3(float _x, float _y, float _z)
{
    x = _x;
    y = _y;
    z = _z;
    magnitude = sqrt(x*x + y*y + z*z);
}


void Vector3::normalize()
{
    float norm = (float)sqrt(x*x + y*y + z*z);

    if(norm==0.0)
    {
        //printf("\n\nNormalize::Divisao por zero");
        x = 0;
        y = 0;
        z = 0;
        return;
    }
    x /= norm;
    y /= norm;
    z /= norm;
    magnitude = 1.0;
}

Vector3 Vector3::normalized()
{
    Vector3 v(x, y, z);
    v.normalize();
    return v;
}

float Vector3::distance(Vector3 point)
{
    return sqrt( pow(point.x - x, 2) + pow(point.y - y, 2) + pow(point.z - z, 2) );
}

Vector3 Vector3::operator + (const Vector3& v)
{
    Vector3 aux( x + v.x, y + v.y, z + v.z);
    return( aux );
}

Vector3& Vector3::operator += (const Vector3& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
}

Vector3 Vector3::operator - (const Vector3& v)
{
    Vector3 aux( x - v.x, y - v.y, z - v.z);
    return( aux );
}

Vector3& Vector3::operator -= (const Vector3& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
}

Vector3 Vector3::operator * (float value)
{
    Vector3 aux( x * value, y * value, z * value);
    return( aux );
}

Vector3& Vector3::operator *= (float value)
{
    x *= value;
    y *= value;
    z *= value;
    return *this;
}

Vector3 Vector3::operator / (float value)
{
    Vector3 aux( x / value, y / value, z / value);
    return( aux );
}

Vector3& Vector3::operator /= (float value)
{
    x /= value;
    y /= value;
    z /= value;
    return *this;
}

float Vector3::operator * (const Vector3& v)
{
    return( x*v.x + y*v.y + z*v.z );
}

float Vector3::dot(const Vector3& v)
{
    return( x*v.x + y*v.y + z*v.z );
}

Vector3 Vector3::operator ^ (const Vector3& v)
{
    Vector3 aux( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    return( aux );
}

Vector3& Vector3::operator ^= (const Vector3& v)
{
    float x_aux = y*v.z - z*v.y;
    float y_aux = z*v.x - x*v.z;
    z = x*v.y - y*v.x;
    x = x_aux;
    y = y_aux;
    return *this;
}

Vector3 Vector3::cross(const Vector3& v)
{
    Vector3 aux( y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
    return( aux );
}

Vector3 Vector3::multiply(const Vector3& v)
{
    Vector3 aux( x*v.x, y*v.y, z*v.z);
    return( aux );
}


std::ostream& operator<<(std::ostream& os, const Vector3& obj)
{
    os << "(" << obj.x << ", " << obj.y << ", " << obj.z << ")";
    return os;
}

bool Vector3::operator == (const Vector3& v)
{
    float epsilon = 0.00001f;
    return( fabs(x - v.x) < epsilon && fabs(y - v.y) < epsilon && fabs(z - v.z) < epsilon );
}

bool Vector3::operator != (const Vector3& v)
{
    return( !(*this == v) );
}

Vector3 Vector3::fromHex(uint32_t hex){
    int r = (hex >> 16) & 0xFF;
    int g = (hex >> 8) & 0xFF;
    int b = hex & 0xFF;
    return Vector3(r/255.0, g/255.0, b/255.0);
}
