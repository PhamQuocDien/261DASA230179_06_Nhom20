#include "JsonDatabase.h"
#include <fstream>
#include <iostream>
using namespace std;
using nlohmann::json;
JsonDatabase::JsonDatabase(const string& filePath) {
    this->filePath = filePath;
}
json JsonDatabase::load() {
    ifstream file(this->filePath);
    if (!file.is_open()) {
        cerr << "Khong the mo file: " << this->filePath << endl;
        return {};
    }
    json data;
    try {
        file >> data;
    }
    catch (const exception& e) {
        cerr << "JSON error: " << e.what() << endl;
        return {};
    }
    file.close();
    return data;
}
bool JsonDatabase::save(const json& data) {
    try {
        ofstream file(this->filePath);
        if (!file.is_open()) {
            cerr << "Khong the mo file de ghi: " << this->filePath << endl;
            return false;
        }
        file << data.dump(4);
        file.close();
        return true;
    }
    catch (const exception& e) {
        cerr << "Save error: " << e.what() << endl;
        return false;
    }
}