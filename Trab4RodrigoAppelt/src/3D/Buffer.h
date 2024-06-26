#ifndef __BUFFER_H__
#define __BUFFER_H__

/// @brief Buffer de tela.
class Buffer{
public:
    Buffer(int width, int height, int channels);
    ~Buffer();

    inline float* getBuffer() const{
        return buffer;
    }
    inline int getOffset(int x, int y) const{
        return (y * width + x) * channels;
    }

    void display(float upscale = 1) const;
    void fill(float red, float green, float blue);
    void fill(float gray);

    /// @brief Redimensiona o buffer e apaga seus conteudos
    void resize(int width, int height);

    /// @brief Redimensiona o buffer se houve mudanca nos tamanhos.
    void resizeIfNeeded(int width, int height);

    void getSize(float &width, float &height) const;
private:
    int width, height;
    int channels;

    float *buffer;
    void displayColor(float upscale) const;
    void displayGray(float upscale) const;
};

#endif