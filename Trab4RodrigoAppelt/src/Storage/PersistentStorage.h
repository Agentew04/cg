#ifndef __PERSISTENT_STORAGE_H__
#define __PERSISTENT_STORAGE_H__

#include <string>
#include <map>
#include <iostream>
#include <iomanip>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

/// @brief Classe que controla a persistencia de dados.
/// @details Segmenta eles em 'containers' nomeados e permite
/// a serializacao de tipos basicos numericos.
/// As implementacoes sao genericas exceto para tipos
/// std(nesse caso ha uma especializacao). Suporta
/// comentarios com '#' ou ';', porem vai salvar
/// todos no inicio do arquivo.
/// @remarks Cada dado eh separado em uma tupla:
/// (container, chave, tamanho, metadados, binario)
class PersistentStorage {
public:

    /// @brief Carrega dados salvos do disco ou
    /// inicia um novo save.
    static void load(std::string savePath);
    /// @brief Salva o estado atual dos dados
    /// no disco para ser usado em outra run.
    static void save();

    /// @brief Deleta todos os dados salvos.
    static void reset();

    template<typename T>
    static void set(std::string container, std::string key, std::string value){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return;
        }
        if(value.size() > 2047){
            std::cout << "String too big: " << value.size() << std::endl;
            return;
        }

        // create container if not exists yet
        if(containers.find(container) == containers.end()){
            containers[container] = new Container();
        }

        // if already exists, free
        if(containers[container]->heap.find(key) != containers[container]->heap.end()){
            delete (char*)containers[container]->heap[key];
        }
        char *charPtr = new char[value.size()+1];
        std::copy(value.begin(), value.end(), charPtr);
        charPtr[value.size()] = '\0';
        uint32_t metadata = 0;
        metadata |= 0b1011 << 28; // set string identifier
        metadata |= (value.size() & 0xFFF) << 16; // set string size
        // os 16 bits low sao reservados se precisar dps
        containers[container]->metadatas[charPtr] = metadata;
        containers[container]->heap[key] = charPtr;
        containers[container]->heapSizes[charPtr] = value.size()+1;
    }

    template<typename T>
    static std::string get(std::string container, std::string key, std::string){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return "";
        }

        if(containers.find(container) == containers.end()){
            return "";
        }
        if(containers[container]->heap.find(key) == containers[container]->heap.end()){
            return "";
        }
        char* ptr = (char*)containers[container]->heap[key];
        uint32_t metadata = containers[container]->metadatas[ptr];
        if((metadata >> 28) != 0b1011){
            std::cout << "Not a string!" << std::endl;
            std::cout << "meta: " << std::hex << metadata << std::dec << std::endl;
            return "";
        }
        uint32_t size = (metadata >> 16) & 0xFFF;

        std::string value(ptr, size);
        return value;
    }

    template<typename T>
    static void set(std::string container, std::string key, std::vector<T> value){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return;
        }

        if(value.size() > 65536){
            std::cout << "Array too big: " << value.size() << std::endl;
            return;
        }

        // create container if not exists yet
        if(containers.find(container) == containers.end()){
            containers[container] = new Container();
        }

        // if already exists, free
        if(containers[container]->heap.find(key) != containers[container]->heap.end()){
            delete[] (T*)containers[container]->heap[key];
        }

        T *array = new T[value.size()];
        std::copy(value.begin(), value.end(), array);
        uint32_t metadata = 0;
        // highest 4 bits are array identifier 1010
        metadata |= 0b1010 << 28;
        // low 16 bits are array size
        metadata |= value.size() & 0xFFFF;
        // hightest 14 bits after identifier are item size
        metadata |= (sizeof(T) & 0xFFF) << 16;
        containers[container]->metadatas[array] = metadata;
        containers[container]->heap[key] = array;
        containers[container]->heapSizes[array] = value.size() * sizeof(T);
    }

    template<typename T>
    static std::vector<T> get(std::string container, std::string key, std::vector<T>){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return {};
        }

        if(containers.find(container) == containers.end()){
            return {};
        }
        if(containers[container]->heap.find(key) == containers[container]->heap.end()){
            return {};
        }
        T* ptr = (T*)containers[container]->heap[key];
        uint32_t metadata = containers[container]->metadatas[ptr];
        // check array identifier
        if((metadata >> 28) != 0b1010){
            std::cout << "Not an array! Identifier was: " << (metadata >> 28) << ". Was supposed to be: "  << 0b1010 << "." << std::endl;
            return {};
        }
        uint32_t elements = metadata & 0xFFFF;
        uint32_t itemSize = (metadata >> 16) & 0xFFF;
        if(itemSize != sizeof(T)){
            std::cout << "Item size mismatch! Expected: " << sizeof(T) << ". Got: " << itemSize << "." << std::endl;
        }
        std::vector<T> value(elements);
        std::copy(ptr, ptr+elements, value.begin());
        return value;
    }

    // T
    template<typename T>
    static T get(std::string container, std::string key, T){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return T();
        }

        if(containers.find(container) == containers.end()){
            return T();
        }
        if(containers[container]->heap.find(key) == containers[container]->heap.end()){
            std::cout << "Key not found: " << key << std::endl;
            return T();
        }
        void* ptr = containers[container]->heap[key];
        T value = *((T*)ptr);
        return value;
    }

    template<typename T>
    static void set(std::string container, std::string key, T value){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return;
        }


        // create container if not exists yet
        if(containers.find(container) == containers.end()){
            containers[container] = new Container();
        }
        // if already exists, free
        if(containers[container]->heap.find(key) != containers[container]->heap.end()){
            delete (T*)containers[container]->heap[key];
        }
        void *ptr = new T(value);
        containers[container]->heap[key] = ptr;
        containers[container]->heapSizes[ptr] = sizeof(T);
    }

    static bool has(std::string container, std::string key){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return false;
        }

        if(containers.find(container) == containers.end()){
            return false;
        }
        if(containers[container]->heap.find(key) == containers[container]->heap.end()){
            return false;
        }
        return containers[container]->heap[key] != nullptr;
    }

    template<typename T>
    static void remove(std::string container, std::string key){
        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return T();
        }

        if(containers.find(container) == containers.end()){
            return;
        }
        if(containers[container]->heap.find(key) == containers[container]->heap.end()){
            return;
        }
        void *ptr = containers[container]->heap[key];
        containers[container]->heapSizes.erase(ptr);
        containers[container]->heap.erase(key);
        containers[container]->metadatas.erase(ptr);
        delete (T*)ptr;
    }

    template<typename T>
    static void setIfNot(std::string container, std::string key, T value){

        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return;
        }

        if(has(container, key) == false){
            set<T>(container, key, value);
        }
    }

    template<typename T>
    static T getOrSetDefault(std::string container, std::string key, T defaultValue){

        if(!isValidIdentifier(container, key)){
            std::cout << "Invalid id!" << std::endl;
            return T();
        }

        if(has(container, key) == false){
            set<T>(container, key, defaultValue);
        }
        if(std::is_same<T, std::string>::value){
            std::string val = get<std::string>(container, key, std::string());
            return *(T*)&val;
        }else{
            return get<T>(container, key, T());
        }
    }

private:

    /// @brief Estrutura que armazena os comentarios no arquivo.
    /// Usado puramente para informacao caso o arquivo seja aberto
    /// para debug(ou curiosidade kkjk)
    static std::vector<std::string> comments;

    class Container {
    public:
        ~Container();
        std::map<std::string, void*> heap;
        // tamanho de dados de cada ponteiro
        std::map<void*,uint32_t> heapSizes;
        // int generico para interpretar info de cada tipo
        std::map<void*,uint32_t> metadatas;
    };

    static std::string savePath;

    static std::map<std::string, Container*> containers;

    static bool isValidIdentifier(std::string container, std::string key);
};

#endif
