#ifndef __MATRIX_H__
#define __MATRIX_H__

class Matrix{
public:
    /// @brief Cria nova matrix nxn
    /// @param n A dimensao da matriz
    Matrix(int n);
    /// @brief Copia uma matriz
    /// @param m A matriz a ser copiada
    Matrix(Matrix &m);
    /// @brief Cria uma nova matriz nxn com dados
    /// @param n A dimensao da matriz
    /// @param data Os dados da matriz
    Matrix(int n, float *data);

    /// @brief Destroi a matriz
    ~Matrix();

    /// @brief Retorna o elemento em uma posicao
    /// @param i A coordenada i
    /// @param j A coordenada j
    /// @return O elemento na posicao i,j
    float at(int i, int j);

    // OPERADORES
private:
    float *data;
    int n;
};

#endif