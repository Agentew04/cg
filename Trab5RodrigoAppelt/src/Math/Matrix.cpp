#include "Matrix.h"
#include <string.h>
#include <iostream>

Matrix::Matrix(size_t n)
{
   this->n = n;
   data = new float[n * n];
   memset(data, 0, n * n * sizeof(float));
}

Matrix::Matrix(const Matrix &m)
{
   n = m.n;
   data = new float[n * n];
   memcpy(data, m.data, n * n * sizeof(float));
}

Matrix::Matrix(size_t n, float *data, bool dealloc)
{
   this->n = n;
   this->data = new float[n * n];
   memcpy(this->data, data, n * n * sizeof(float));
   if (dealloc)
   {
      delete[] data;
   }
}

Matrix::~Matrix()
{
   delete[] data;
}

float Matrix::at(uint32_t i, uint32_t j) const
{
   if (i >= n || j >= n)
   {
      std::cout << "Matrix::at: Indice fora dos limites" << std::endl;
      return 0;
   }
   return data[i * n + j];
}

void Matrix::set(uint32_t i, uint32_t j, float val)
{
   if (i >= n || j >= n)
   {
      std::cout << "Matrix::set: Indice fora dos limites" << std::endl;
      return;
   }
   data[i * n + j] = val;
}

// static identity
Matrix Matrix::identity(size_t n)
{
   float *data = new float[n * n];
   memset(data, 0, n * n * sizeof(float));
   for (uint32_t i = 0; i < n; i++)
   {
      data[i * n + i] = 1;
   }
   return Matrix(n, data, true);
}

Matrix Matrix::rotation2D(float angle)
{
   float data[] = {
       (float)cos(angle), (float)-sin(angle),
       (float)sin(angle), (float)cos(angle)};
   return Matrix(2, data);
}

Matrix Matrix::rotation2DHomo(float angle)
{
   float data[] = {
       (float)cos(angle), (float)-sin(angle), 0,
       (float)sin(angle), (float)cos(angle), 0,
       0, 0, 1};
   return Matrix(3, data);
}

Matrix Matrix::translation2D(float x, float y)
{
   float data[] = {
       1, 0, x,
       0, 1, y,
       0, 0, 1};
   return Matrix(3, data);
}

Matrix Matrix::scale2D(float x, float y)
{
   float data[] = {
       x, 0,
       0, y};
   return Matrix(2, data);
}

Matrix Matrix::scale2DHomo(float x, float y)
{
   float data[] = {
       x, 0, 0,
       0, y, 0,
       0, 0, 1};
   return Matrix(3, data);
}

size_t Matrix::dim() const
{
   return n;
}

float Matrix::det() const
{
   if (n == 1)
   {
      return data[0];
   }
   if (n == 2)
   {
      return data[0] * data[3] - data[1] * data[2];
   }
   // sarrus
   if (n == 3)
   {
      return data[0] * data[4] * data[8] + data[1] * data[5] * data[6] + data[2] * data[3] * data[7] - data[2] * data[4] * data[6] - data[1] * data[3] * data[8] - data[0] * data[5] * data[7];
   }
   // recursivo
   float res = 0;
   for (uint32_t i = 0; i < n; i++)
   {
      Matrix sub(n - 1);
      for (uint32_t j = 1; j < n; j++)
      {
         for (uint32_t k = 0; k < n; k++)
         {
            if (k < i)
            {
               sub.data[(j - 1) * n + k] = data[j * n + k];
            }
            else if (k > i)
            {
               sub.data[(j - 1) * n + k - 1] = data[j * n + k];
            }
         }
      }
      res += data[i] * sub.det() * (i % 2 == 0 ? 1 : -1);
   }
   return res;
}

Matrix Matrix::transpose() const
{
   float *data = new float[n * n];
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         data[j * n + i] = this->data[i * n + j];
      }
   }
   return Matrix(n, data, true);
}

// Matrix Matrix::inverse() const
// {
//     /*
//     chatgpt me deu essa resposta, no projeto de testes
//     eu testo se esse codigo esta certo com varios casos.
//     ... nao ta funcionando, vou comentar so, vai q vou usar dps
//     */

//     // Create a new matrix that is the identity matrix of the same size as the original matrix.
//     Matrix inv(n);
//     for (int i = 0; i < n; i++)
//     {
//         inv.data[i * n + i] = 1.0f;
//     }

//     // For each row in the original matrix, do the following:
//     for (int i = 0; i < n; i++)
//     {
//         // If the row is not all zeros, then swap it with the first row.
//         if (data[i * n + 0] == 0.0f)
//         {
//             for (int j = i + 1; j < n; j++)
//             {
//                 if (data[j * n + 0] != 0.0f)
//                 {
//                     for (int k = 0; k < n; k++)
//                     {
//                         float temp = data[i * n + k];
//                         data[i * n + k] = data[j * n + k];
//                         data[j * n + k] = temp;

//                         temp = inv.data[i * n + k];
//                         inv.data[i * n + k] = inv.data[j * n + k];
//                         inv.data[j * n + k] = temp;
//                     }
//                     break;
//                 }
//             }
//         }

//         // Divide each element in the row by the leading coefficient (the first non-zero element in the row).
//         float leading_coeff = data[i * n + 0];
//         for (int j = 0; j < n; j++)
//         {
//             data[i * n + j] /= leading_coeff;
//             inv.data[i * n + j] /= leading_coeff;
//         }

//         // Subtract multiples of the row from the other rows to make all the other elements in the column zero.
//         for (int j = 0; j < n; j++)
//         {
//             if (j != i)
//             {
//                 float multiplier = data[j * n + 0];
//                 for (int k = 0; k < n; k++)
//                 {
//                     data[j * n + k] -= multiplier * data[i * n + k];
//                     inv.data[j * n + k] -= multiplier * inv.data[i * n + k];
//                 }
//             }
//         }
//     }

//     // Return the inverse of the original matrix.
//     return inv;
// }

Matrix Matrix::operator+(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator+: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         res.data[i * n + j] = data[i * n + j] + m.data[i * n + j];
      }
   }
   return res;
}

Matrix &Matrix::operator+=(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator+=: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         data[i * n + j] += m.data[i * n + j];
      }
   }
   return *this;
}

Matrix Matrix::operator-(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator-: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         res.data[i * n + j] = data[i * n + j] - m.data[i * n + j];
      }
   }
   return res;
}

Matrix Matrix::operator-()
{
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         res.data[i * n + j] = -data[i * n + j];
      }
   }
   return res;
}

Matrix &Matrix::operator-=(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator-=: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         data[i * n + j] -= m.data[i * n + j];
      }
   }
   return *this;
}

Matrix Matrix::operator*(const float a)
{
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         res.data[i * n + j] = data[i * n + j] * a;
      }
   }
   return res;
}

Matrix &Matrix::operator*=(const float a)
{
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         data[i * n + j] *= a;
      }
   }
   return *this;
}

Matrix Matrix::operator*(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator*: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         for (uint32_t k = 0; k < n; k++)
         {
            res.data[i * n + j] += data[i * n + k] * m.data[k * n + j];
         }
      }
   }
   return res;
}

Matrix &Matrix::operator*=(const Matrix &m)
{
   if (n != m.n)
   {
      std::cout << "Matrix::operator*=: Matrizes de tamanhos diferentes" << std::endl;
      return *this;
   }
   Matrix res(n);
   for (uint32_t i = 0; i < n; i++)
   {
      for (uint32_t j = 0; j < n; j++)
      {
         for (uint32_t k = 0; k < n; k++)
         {
            res.data[i * n + j] += data[i * n + k] * m.data[k * n + j];
         }
      }
   }
   memcpy(data, res.data, n * n * sizeof(float));
   return *this;
}

Vector3 Matrix::operator*(const Vector3 &v)
{
   if (n != 3)
   {
      std::cout << "Matrix::operator*: Matriz nao 3x3" << std::endl;
      return Vector3(0.0f, 0.0f, 0.0f);
   }
   Vector3 res;
   res.x = data[0] * v.x + data[1] * v.y + data[2] * v.z;
   res.y = data[3] * v.x + data[4] * v.y + data[5] * v.z;
   res.z = data[6] * v.x + data[7] * v.y + data[8] * v.z;
   return res;
}

/// @brief Multiplica uma matriz 2x2 com um vector 2x1
Vector2 Matrix::operator*(const Vector2 &v)
{
   if (n != 2)
   {
      std::cout << "Matrix::operator*: Matriz nao 2x2" << std::endl;
      return Vector2(0.0f, 0.0f);
   }
   Vector2 res = Vector2::zero();
   res.x = data[0] * v.x + data[1] * v.y;
   res.y = data[2] * v.x + data[3] * v.y;
   return res;
}

std::vector<float> Matrix::operator*(const std::vector<float> &v)
{
   if (n != v.size())
   {
      std::cout << "Matrix::operator*: Tamanho da matriz e do vetor incompativeis" << std::endl;
      return std::vector<float>();
   }
   std::vector<float> res(n, 0);
   // for linha do vetor
   for (uint32_t i = 0; i < n; i++)
   {
      // for coluna da matriz
      for (uint32_t j = 0; j < n; j++)
      {
         res[i] += data[i * n + j] * v[j];
      }
   }
   return res;
}

std::ostream &operator<<(std::ostream &os, const Matrix &m)
{
   for (uint32_t i = 0; i < m.n; i++)
   {
      for (uint32_t j = 0; j < m.n; j++)
      {
         os << m.at(i, j) << " ";
      }
      os << std::endl;
   }
   return os;
}