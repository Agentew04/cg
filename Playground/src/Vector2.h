#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <cmath>
#include <iostream>

class Vector2
{
public:
   float x, y;

   Vector2()
   {
      x = y = 0;
   }

   Vector2(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

   static Vector2 zero(){ return Vector2(0,0); }
   static Vector2 one(){ return Vector2(1,1); }
   static Vector2 left(){ return Vector2(-1,0); }
   static Vector2 right(){ return Vector2(1,0); }
   static Vector2 up(){ return Vector2(0,-1); }
   static Vector2 down(){ return Vector2(0,1); }

   void set(float _x, float _y)
   {
       x = _x;
       y = _y;
   }

   Vector2 flip(){
       return Vector2(y,x);
   }

   void normalize()
   {
       float norm = (float)sqrt(x*x + y*y);

       if(norm==0.0)
       {
          std::cout << "\n\nNormalize::Divisao por zero" << std::endl;
          x = 1;
          y = 1;
          return;
       }
       x /= norm;
       y /= norm;
   }
    Vector2 normalized(){
        float x,y;
        float norm = (float)sqrt(this->x*this->x + this->y*this->y);
        if(norm==0.0){
            return Vector2::zero();
        }
        return Vector2(this->x/norm, this->y/norm);

    }

    float distance(Vector2 point){
        return std::sqrt(std::pow(point.x - this->x, 2) + std::pow(point.y - this->y, 2));
    }

    float magnitude(){
        float norm = (float)sqrt(x*x + y*y);
        return norm;
    }

    float dot(const Vector2& v){
        return this->x*v.x + this->y*v.y;
    }

   Vector2 operator - (const Vector2& v)
   {
        Vector2 aux( x - v.x, y - v.y);
        return( aux );
   }

   Vector2 operator + (const Vector2& v)
   {
       Vector2 aux( x + v.x, y + v.y);
       return( aux );
   }

   Vector2 operator += (const Vector2& v)
   {
       Vector2 aux( x + v.x, y + v.y);
       return( aux );
   }

   Vector2 operator -= (const Vector2& v)
   {
       Vector2 aux( x - v.x, y - v.y);
       return( aux );
   }

   Vector2 operator *(const float f){
       Vector2 aux(this->x*f, this->y*f);
       return (aux);
   }

   Vector2 operator *(const Vector2& v){
       Vector2 aux(this->x*v.x, this->y*v.y);
       return (aux);
   }

   Vector2 operator *= (const float f)
   {
       Vector2 aux(this->x*f, this->y*f);
       return (aux);
   }

   bool operator == (const Vector2& v)
   {
       return (this->x == v.x && this->y == v.y);
   }
   bool operator != (const Vector2& v)
   {
       return !(*this == v);
   }

   //Adicionem os demais overloads de operadores aqui.
    friend std::ostream& operator<<(std::ostream& os, const Vector2& obj){
        os << "(" << obj.x << ", " << obj.y << ")";
        return os;
    }

};

#endif
