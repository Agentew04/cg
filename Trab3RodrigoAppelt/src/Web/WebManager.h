#ifndef __WEB_MANAGER_H__
#define __WEB_MANAGER_H__

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace httplib{
    class Client;
}

/// @brief Classe usada pra simplificar o uso
/// de requests HTTP da lib httplib.
/// @remarks Em caso de imcompatibilidade,
/// descomentar a linha #define DISABLE_WEB no arquivo
/// WebManager.cpp. Para tentar corrigir, certifique-se
/// de incluir a flag -lws2_32 no compilador.
class WebManager{
public:

    /// @brief Realiza um GET request para uma url.
    /// @param domain Dominio da url. Nao incluir http://
    /// @param path Caminho da url. Comeca com /
    /// @param queryParams Parametros da url
    /// @param success Ponteiro para variavel que vai guardar se a request foi bem sucedida
    /// @return Body da resposta do servidor
    /// @remarks Apenas manda requests HTTP para porta 80.
    /// Nao suporta HTTPS.
    static std::string get(
        std::string domain,
        std::string path,
        std::map<std::string, std::string> queryParams,
        bool *success);

    /// @brief Faz um POST request para uma url. Os
    /// parametros sao traduzidos para um body em json.
    /// @param domain Dominio da url. Nao incluir http://
    /// @param path Caminho da url. Comeca com /
    /// @param queryParams Parametros passados
    /// @param success Ponteiro para variavel que vai guardar se a request foi bem sucedida
    /// @return Body da resposta do servidor
    static std::string post(
        std::string domain,
        std::string path,
        std::map<std::string, std::string> queryParams,
        bool *success);

    /// @brief Libera recursos usados pelo WebManager
    static void free();

    /// @brief Realiza parse de um objeto json simples.
    static std::map<std::string, std::string> fromJsonObject(const std::string& json);

    /// @brief Realiza parse de um array de objetos json simples.
    static std::vector<std::map<std::string, std::string>> fromJsonArray(const std::string& jsonArray);
private:
    static std::string buildQueryParams(std::string path,
        std::map<std::string, std::string> queryParams);

    static std::string buildJsonBody(std::map<std::string, std::string> queryParams);

    static std::map<
        std::string,
        std::unique_ptr<httplib::Client>
    > clients;
};

#endif
