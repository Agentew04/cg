#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <string.h>
#include <iostream>

#include "../Vector2.h"
#include "../Vector3.h"

class Matrix{
public:
    /// @brief Cria nova matrix nxn
    /// @param n A dimensao da matriz
    Matrix(int n){
        this->n = n;
        data = new float[n*n];
        memset(data, 0, n*n*sizeof(float));
        std::cout << "Alloc matrix " << n << std::endl;
    }

    /// @brief Copia uma matriz
    /// @param m A matriz a ser copiada
    Matrix(Matrix &m){
        this->n = m.n;
        data = new float[n*n];
        memcpy(data, m.data, n*n*sizeof(float));
        std::cout << "Alloc matrix " << n << std::endl;
    }

    /// @brief Cria uma nova matriz nxn com dados
    /// @param n A dimensao da matriz
    /// @param data Os dados da matriz
    Matrix(int n, float *data, bool dealloc = false){
        this->n = n;
        this->data = new float[n*n];
        memcpy(this->data, data, n*n*sizeof(float));
        std::cout << "Alloc matrix " << n << std::endl;
        if(dealloc){
            delete[] data;
        }
    }

    /// @brief Destroi a matriz
    ~Matrix(){
        delete[] data;
        std::cout << "Dealloc matrix " << n << std::endl;
    }

    /// @brief Retorna o elemento em uma posicao
    /// @param i A coordenada i
    /// @param j A coordenada j
    /// @return O elemento na posicao i,j
    float at(int i, int j) const{
        if(i < 0 || i >= n || j < 0 || j >= n){
            std::cout << "Matrix::at: Indice fora dos limites" << std::endl;
            return 0;
        }
        return data[i*n + j];
    }

    /// @brief Retorna uma matriz identidade
    /// @param n o tamanho da matriz
    /// @return a matriz criada
    static Matrix identity(int n){
        float *data = new float[n*n];
        memset(data, 0, n*n*sizeof(float));
        for(int i = 0; i < n; i++){
            data[i*n + i] = 1;
        }
        return Matrix(n, data);
    }

    /// @brief Retorna a dimensao da matriz quadrada
    int dim() const{
        return n;
    }

    /// @brief Calcula o determinante da matriz
    float det() const {
        if(n==1){
            return data[0];
        }
        if(n==2){
            return data[0]*data[3] - data[1]*data[2];
        }
        // sarrus
        if(n==3){
            return data[0]*data[4]*data[8] + data[1]*data[5]*data[6] + data[2]*data[3]*data[7] - data[2]*data[4]*data[6] - data[1]*data[3]*data[8] - data[0]*data[5]*data[7];
        }
        // recursivo
        float res = 0;
        for(int i = 0; i < n; i++){
            Matrix sub(n-1);
            for(int j = 1; j < n; j++){
                for(int k = 0; k < n; k++){
                    if(k < i){
                        sub.data[(j-1)*n + k] = data[j*n + k];
                    }else if(k > i){
                        sub.data[(j-1)*n + k-1] = data[j*n + k];
                    }
                }
            }
            res += data[i] * sub.det() * (i%2==0?1:-1);
        }
        return res;
    }

    /// @brief Printa a matriz em uma stream
    friend std::ostream& operator <<(std::ostream &os, const Matrix &m){
        for(int i = 0; i < m.n; i++){
            for(int j = 0; j < m.n; j++){
                os << m.at(i,j) << " ";
            }
            os << std::endl;
        }
        return os;
    }

    /// @brief Soma duas matrizes de mesmo tamanho termo a termo
    Matrix operator +(const Matrix &m){
        if(n != m.n){
            std::cout << "Matrix::operator+: Matrizes de tamanhos diferentes" << std::endl;
            return *this;
        }
        Matrix res(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                res.data[i*n + j] = data[i*n + j] + m.data[i*n + j];
            }
        }
        return res;
    }

    /// @brief Subtrai duas matrizes termo a termo
    Matrix operator -(const Matrix &m){
        if(n != m.n){
            std::cout << "Matrix::operator-: Matrizes de tamanhos diferentes" << std::endl;
            return *this;
        }
        Matrix res(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                res.data[i*n + j] = data[i*n + j] - m.data[i*n + j];
            }
        }
        return res;
    }

    /// @brief Multiplica cada termo por um escalar a
    Matrix operator *(const float a){
        Matrix res(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                res.data[i*n + j] = data[i*n + j] * a;
            }
        }
        return res;
    }

    /// @brief Multiplica duas matrizes
    Matrix operator *(const Matrix &m){
        if(n != m.n){
            std::cout << "Matrix::operator*: Matrizes de tamanhos diferentes" << std::endl;
            return *this;
        }
        Matrix res(n);
        for(int i = 0; i < n; i++){
            for(int j = 0; j < n; j++){
                for(int k = 0; k < n; k++){
                    res.data[i*n + j] += data[i*n + k] * m.data[k*n + j];
                }
            }
        }
        return res;
    }

    /// @brief multiplica uma matriz 3x3 com um vector 3x1
    Vector3 operator *(const Vector3 &v){
        if(n != 3){
            std::cout << "Matrix::operator*: Matriz nao 3x3" << std::endl;
            return Vector3(0.0f, 0.0f, 0.0f);
        }
        Vector3 res;
        for(int i = 0; i < 3; i++){
            res.x += data[i*3 + 0] * v.x;
            res.y += data[i*3 + 1] * v.y;
            res.z += data[i*3 + 2] * v.z;
        }
        return res;
    }

    /// @brief Multiplica uma matriz 2x2 com um vector 2x1
    Vector2 operator *(const Vector2 &v){
        if(n != 2){
            std::cout << "Matrix::operator*: Matriz nao 2x2" << std::endl;
            return Vector2(0.0f, 0.0f);
        }
        Vector2 res;
        for(int i = 0; i < 2; i++){
            res.x += data[i*2 + 0] * v.x;
            res.y += data[i*2 + 1] * v.y;
        }
        return res;
    }
    

private:
    float *data;
    int n;
};

#endif