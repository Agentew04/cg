#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <string>
#include <vector>
#include <map>
#include "../Math/Vector3.h"

/// @brief Estrutura que guarda as informacoes de um arquivo .obj
struct ObjFile {
public:
    /// @brief Lista com os vertices do modelo
    std::vector<Vector3> vertices;
    /// @brief Lista com as faces do modelo
    std::vector<std::vector<int>> faces;
    /// @brief Posicao do modelo
    Vector3 scale;
};

/// @brief Classe que carrega e salva na memoria interna
/// arquivos .obj simples
/// @remark Apenas suporta arquivos .obj com vertices e faces.
/// Nao suporta texturas, normais, multiplos objetos no mesmo arquivo, etc
class ObjLoader {
public:

    /// @brief Carrega um arquivo .obj para a memoria
    /// @param filename O nome do arquivo
    /// @param id O identificador do arquivo
    static void load(const std::string& filename, const std::string& id);

    /// @brief Libera todos os arquivos .obj da memoria
    static void free();

    /// @brief Pega um arquivo .obj da memoria ja carregado
    /// @param id O identificador do arquivo
    static ObjFile* get(const std::string& id);
private:
    static std::map<std::string, ObjFile*> objFiles;
};

#endif // __OBJLOADER_H__