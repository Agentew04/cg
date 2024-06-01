#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <string>
#include <vector>
#include <map>
#include "../Math/Vector3.h"

/// @brief Um struct que contem informacoes
/// sobre um modelo 3d
struct Model3D {
    std::vector<Vector3> vertices;
    std::vector<Vector3> normals;
    std::vector<std::vector<int>> faces;
    std::vector<std::vector<int>> faceNormals;
    bool hasNormalData;
};

/// @brief Classe que carrega e salva na memoria interna
/// arquivos .obj simples
/// @remark Apenas suporta arquivos .obj com multiplos objetos
/// contendo vertices, faces e normais.
/// Nao suporta texturas, materiais(arquivos .mtl) e cores.
class ObjLoader {
public:

    static void load(const std::string& filename, const std::string& fileId);

    /// @brief Libera todos os arquivos .obj da memoria
    static void free();

    /// @brief Pega um arquivo .obj da memoria ja carregado
    /// @param id O identificador do arquivo
    static Model3D* get(const std::string& id, const std::string& objectName = "");

    static std::vector<std::string> getModelNames(const std::string& id);
    static std::vector<std::string> getIds();
private:
    static std::map<std::string, std::map<std::string, Model3D>> models;

    struct obj{
        std::string name;
        std::vector<Vector3> vertices;
        std::vector<Vector3> normals;
        std::vector<std::vector<int>> faces;
        std::vector<std::vector<int>> faceNormals;
    };
};

#endif // __OBJLOADER_H__