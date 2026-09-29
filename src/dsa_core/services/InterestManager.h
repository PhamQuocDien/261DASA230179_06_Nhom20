#ifndef INTERESTMANAGER_H
#define INTERESTMANAGER_H

#include <vector>
#include <string>
#include "../models/BookInterest.h"

class InterestManager {
private:
    std::vector<BookInterest> records;
    int findByCode(const std::string& bookCode) const;

public:
    void loadFromBookData(const std::string& bookCode, int totalQuantity, int availableCount);
    void recordPurchase(const std::string& bookCode);
    void recordReturn(const std::string& bookCode);
    int getInterestScore(const std::string& bookCode) const;
    std::vector<BookInterest> getRankedByInterest() const;
    void syncFromJson(const std::string& bookCode, int newQuantity, int newAvailable);
};

#endif 
