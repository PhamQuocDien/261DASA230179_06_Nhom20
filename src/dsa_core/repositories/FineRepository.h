#ifndef FINEREPOSITORY_H
#define FINEREPOSITORY_H
#include <string>
#include <vector>
#include "../models/Fine.h"
class FineRepository {
private:
    std::vector<Fine> fines;
public:
    FineRepository();
    std::vector<Fine>& getAll();
    const std::vector<Fine>& getAll() const;
    Fine* findById(const std::string& fineId);
    const Fine* findById(const std::string& fineId) const;
    bool add(const Fine& fine);
    bool update(const Fine& fine);
    bool removeById(const std::string& fineId);
};
#endif