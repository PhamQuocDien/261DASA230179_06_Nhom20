#ifndef JSONDATABASE_H
#define JSONDATABASE_H
#include <string>
#include <nlohmann/json.hpp>
class JsonDatabase {
private:
    std::string filePath;
public:
    JsonDatabase(const std::string& filePath);
    nlohmann::json load();
    bool save(const nlohmann::json& data);
};
#endif