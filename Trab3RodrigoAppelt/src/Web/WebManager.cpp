#include "WebManager.h"

#include <memory>

/*
Se quiser desabilitar toda a funcionalidade online
do trabalho, descomente a linha abaixo
*/

// #define DISABLE_WEB

#include "httplib.h"

std::map<std::string, std::unique_ptr<httplib::Client>> WebManager::clients;

std::string WebManager::get(
    std::string domain,
    std::string path,
    std::map<std::string, std::string> queryParams,
    bool *success){

    // get client
    if(clients.find(domain) == clients.end()){
        clients[domain] = std::make_unique<httplib::Client>(domain, 80);
    }

    httplib::Client &cli = *clients[domain];

    std::string url = buildQueryParams(path, queryParams);

    #ifndef DISABLE_WEB    
    auto res = cli.Get(url);

    if(!res){
        auto err = res.error();
        if(success)
            *success = false;
        std::cout << "Web Error: " << err << std::endl;
        return "";
    }

    if(res->status != httplib::StatusCode::OK_200){
        if(success)
            *success = false;
        std::cout << "Status code: " << res->status << std::endl;
        return "";
    }

    *success = true;
    return res->body;
    #else
    if(success)
        *success = false;
    return "";
    #endif
}


std::string WebManager::post(
    std::string domain,
    std::string path,
    std::map<std::string, std::string> queryParams,
    bool *success){

    // get client
    if(clients.find(domain) == clients.end()){
        clients[domain] = std::make_unique<httplib::Client>(domain, 80);
    }

    httplib::Client &cli = *clients[domain];

    // create json body
    std::string body = buildJsonBody(queryParams);

    #ifndef DISABLE_WEB
    auto res = cli.Post(path, body, "application/json");

    if(!res){
        auto err = res.error();
        if(success)
            *success = false;
        std::cout << "Web Error: " << err << std::endl;
        return "";
    }

    if(res->status != httplib::StatusCode::OK_200){
        if(success)
            *success = false;
        std::cout << "Status code: " << res->status << std::endl;
        return "";
    }

    *success = true;
    return res->body;
    #else
    if(success)
        *success = false;
    return "";
    #endif

}

std::string WebManager::buildQueryParams(std::string path,
    std::map<std::string, std::string> queryParams){
    std::string result = path;
    if(queryParams.size() > 0){
        result += "?";
        for(auto it = queryParams.begin(); it != queryParams.end(); it++){
            result += it->first + "=" + it->second + "&";
        }
        result.pop_back();
    }
    return result;
}

void WebManager::free(){
    for(auto it = clients.begin(); it != clients.end(); it++){
        #ifndef DISABLE_WEB
        it->second->stop();
        #endif
        delete it->second.release();
    }
    clients.clear();
}

std::string WebManager::buildJsonBody(std::map<std::string, std::string> queryParams){
    std::string output = "{";
    for(auto it = queryParams.begin(); it != queryParams.end(); it++){
        output += "\"" + it->first + "\":\"" + it->second + "\",";
    }
    if(queryParams.size() > 0){
        output.pop_back();
    }
    output += "}";
    return output;
}

std::map<std::string, std::string> WebManager::fromJsonObject(const std::string& json){
    std::map<std::string, std::string> resultMap;

    size_t pos = 1;
    while (pos < json.size() - 1) {
        size_t keyStart = json.find('"', pos) + 1;
        size_t keyEnd = json.find('"', keyStart);
        std::string key = json.substr(keyStart, keyEnd - keyStart);

        size_t valueStart = json.find(':', keyEnd) + 1;
        size_t valueEnd = json.find_first_of(",}", valueStart);
        std::string value = json.substr(valueStart, valueEnd - valueStart);

        if (value.front() == '"') {
            value.erase(0, 1);
            value.erase(value.size() - 1, 1);
        }

        resultMap[key] = value;

        pos = valueEnd + 1;
    }

    return resultMap;
}

std::vector<std::map<std::string, std::string>> WebManager::fromJsonArray(const std::string& jsonArray){
    std::vector<std::map<std::string, std::string>> resultVector;

    size_t pos = 1;
    while (pos < jsonArray.size() - 1) {
        size_t objStart = jsonArray.find('{', pos);
        size_t objEnd = jsonArray.find('}', objStart);
        std::string jsonObject = jsonArray.substr(objStart, objEnd - objStart + 1);

        resultVector.push_back(fromJsonObject(jsonObject));

        pos = objEnd + 2;
    }

    return resultVector;
}