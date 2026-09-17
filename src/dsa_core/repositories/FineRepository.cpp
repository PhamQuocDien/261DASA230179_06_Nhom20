#include "FineRepository.h"
FineRepository::FineRepository() {}
std::vector<Fine>& FineRepository::getAll() {
    return fines;
}
const std::vector<Fine>& FineRepository::getAll() const {
    return fines;
}
Fine* FineRepository::findById(const std::string& fineId) {
    for (Fine& fine : fines) {
        if (fine.fineId == fineId) {
            return &fine;
        }
    }
    return nullptr;
}
const Fine* FineRepository::findById(const std::string& fineId) const {
    for (const Fine& fine : fines) {
        if (fine.fineId == fineId) {
            return &fine;
        }
    }
    return nullptr;
}
bool FineRepository::add(const Fine& fine) {
    if (findById(fine.fineId) != nullptr) {
        return false;
    }
    fines.push_back(fine);
    return true;
}
bool FineRepository::update(const Fine& fine) {
    Fine* existingFine = findById(fine.fineId);
    if (existingFine == nullptr) {
        return false;
    }
    *existingFine = fine;
    return true;
}
bool FineRepository::removeById(const std::string& fineId) {
    for (auto it = fines.begin(); it != fines.end(); ++it) {
        if (it->fineId == fineId) {
            fines.erase(it);
            return true;
        }
    }
    return false;
}