#ifndef __PERSISTENT_STORAGE_H__
#define __PERSISTENT_STORAGE_H__

#include <string>
#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

/// @brief Classe que controla a persistencia de dados.
/// Segmenta eles em 'containers' nomeados e permite
/// a serializacao de tipos basicos numericos.
/// As implementacoes de cada tipo estao nos arquivos:
/// PersistentStorage.TIPO.cpp
class PersistentStorage {
public:
    
    /// @brief Carrega dados salvos do disco ou
    /// inicia um novo save.
    static void load();
    /// @brief Salva o estado atual dos dados
    /// no disco para ser usado em outra run.
    static void save();

    /// @brief Deleta todos os dados salvos.
    static void reset();

    // int
    static void getInt(std::string container, std::string key, int *value);
    static void setInt(std::string container, std::string key, int value);
    static bool hasInt(std::string container, std::string key);
    static void removeInt(std::string container, std::string key);
    static void setIfNotInt(std::string container, std::string key, int value);
    static void getOrDefaultInt(std::string container, std::string key, int *value, int defaultValue);

    // float
    static void getFloat(std::string container, std::string key, float *value);
    static void setFloat(std::string container, std::string key, float value);
    static bool hasFloat(std::string container, std::string key);
    static void removeFloat(std::string container, std::string key);
    static void setIfNotFloat(std::string container, std::string key, float value);
    static void getOrDefaultFloat(std::string container, std::string key, float *value, float defaultValue);

    // Vector2
    static void getVec2(std::string container, std::string key, Vector2 *value);
    static void setVec2(std::string container, std::string key, Vector2 value);
    static bool hasVec2(std::string container, std::string key);
    static void removeVec2(std::string container, std::string key);
    static void setIfNotVec2(std::string container, std::string key, Vector2 value);
    static void getOrDefaultVec2(std::string container, std::string key, Vector2 *value, Vector2 defaultValue);

    // vector3
    static void getVec3(std::string container, std::string key, Vector3 *value);
    static void setVec3(std::string container, std::string key, Vector3 value);
    static bool hasVec3(std::string container, std::string key);
    static void removeVec3(std::string container, std::string key);
    static void setIfNotVec3(std::string container, std::string key, Vector3 value);
    static void getOrDefaultVec3(std::string container, std::string key, Vector3 *value, Vector3 defaultValue);

private:

    class Container {
    public:
        std::map<std::string, int> ints;
        std::map<std::string, float> floats;
        std::map<std::string, Vector2> vec2s;
        std::map<std::string, Vector3> vec3s;
    };

    static std::string savePath;
    static bool initialized;

    static std::map<std::string, Container*> containers;
    
    static void init();
};

#endif