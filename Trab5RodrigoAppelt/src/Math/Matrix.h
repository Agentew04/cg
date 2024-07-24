#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <string.h>
#include <iostream>
#include <vector>

#include "Vector2.h"
#include "Vector3.h"

/// @brief Classe que implementa operacoes sobre matrizes
class Matrix
{
public:
   /// @brief Cria nova matrix nxn
   /// @param n A dimensao da matriz
   Matrix(size_t n);

   /// @brief Copia uma matriz
   /// @param m A matriz a ser copiada
   Matrix(const Matrix &m);

   /// @brief Cria uma nova matriz nxn com dados
   /// @param n A dimensao da matriz
   /// @param data Os dados da matriz
   Matrix(size_t n, float *data, bool dealloc = false);

   /// @brief Destroi a matriz
   ~Matrix();

   /// @brief Retorna o elemento em uma posicao
   /// @param i A coordenada i
   /// @param j A coordenada j
   /// @return O elemento na posicao i,j
   float at(uint32_t i, uint32_t j) const;

   /// @brief Seta um elemento na posicao i,j
   /// @param i A coordenada i
   /// @param j A coordenada j
   /// @param val O valor a ser setado
   void set(uint32_t i, uint32_t j, float val);

   /// @brief Retorna uma matriz identidade
   /// @param n o tamanho da matriz
   /// @return a matriz criada
   static Matrix identity(size_t n);

   /// @brief Cria uma matriz de rotacao 2D
   /// @param angle Angulo em radianos
   static Matrix rotation2D(float angle);
   /// @brief Cria uma matriz de rotacao 2D com coordenadas homogeneas
   /// @param angle Angulo em radianos
   static Matrix rotation2DHomo(float angle);

   /// @brief Cria uma matriz de translacao 2D
   /// @param x Movimento no eixo x
   /// @param y Movimento no eixo y
   static Matrix translation2D(float x, float y);

   /// @brief Cria uma matriz de escala 2D 2x2
   /// @param x O fator de escala no eixo x
   /// @param y O fator de escala no eixo y
   static Matrix scale2D(float x, float y);

   /// @brief Cria uma matriz de escala 2D 3x3 com coordenadas homogeneas
   /// @param x O fator de escala no eixo x
   /// @param y O fator de escala no eixo y
   static Matrix scale2DHomo(float x, float y);

   /// @brief Retorna a dimensao da matriz quadrada
   size_t dim() const;

   /// @brief Calcula o determinante da matriz
   float det() const;

   /// @brief Cria uma matriz transposta desta
   Matrix transpose() const;

   // /// @brief Calcula a matriz inversa desta
   // Matrix inverse() const;

   /// OPERADORES

   /// @brief Printa a matriz em uma stream
   friend std::ostream &operator<<(std::ostream &os, const Matrix &m);

   /// @brief Soma duas matrizes de mesmo tamanho termo a termo
   Matrix operator+(const Matrix &m);
   Matrix &operator+=(const Matrix &m);

   /// @brief Subtrai duas matrizes termo a termo
   Matrix operator-(const Matrix &m);
   /// @brief Multiplica cada termo da matrix por -1
   Matrix operator-();
   /// @brief Subtrai uma matriz de outra inplace
   Matrix &operator-=(const Matrix &m);

   /// @brief Multiplica cada termo por um escalar a
   Matrix operator*(const float a);
   /// @brief Multiplica cada termo por um escalar a inplace
   Matrix &operator*=(const float a);
   /// @brief Multiplica duas matrizes
   Matrix operator*(const Matrix &m);
   /// @brief Multiplica duas matrizes inplace
   Matrix &operator*=(const Matrix &m);

   /// @brief multiplica uma matriz 3x3 com um vector 3x1
   Vector3 operator*(const Vector3 &v);

   /// @brief Multiplica uma matriz 2x2 com um vector 2x1
   Vector2 operator*(const Vector2 &v);

   /// @brief Multiplica uma matriz nxn com um vector nx1
   std::vector<float> operator*(const std::vector<float> &v);

private:
   float *data;
   size_t n;
};

#endif
