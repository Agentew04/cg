#ifndef __VECTOR_2_H__
#define __VECTOR_2_H__

#include <cmath>

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
          printf("\n\nNormalize::Divisao por zero");
          x = 1;
          y = 1;
          return;
       }
       x /= norm;
       y /= norm;
   }

   float distance(Vector2 point){
       return std::sqrt(std::pow(point.x - this->x, 2) + std::pow(point.y - this->y, 2));
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

   //Adicionem os demais overloads de operadores aqui.


};

#endif
