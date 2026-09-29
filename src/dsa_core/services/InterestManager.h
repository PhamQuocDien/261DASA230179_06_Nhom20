#ifndef INTERESTMANAGER_H
#define INTERESTMANAGER_H

#include <vector>
#include <algorithm>
#include <string>
#include "BookInterest.h"

class InterestManager {
private:
    std::vector<BookInterest> records;

    int findByCode(const std::string& bookCode) const {
        for (size_t i = 0; i < records.size(); ++i) {
            if (records[i].bookCode == bookCode) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    void loadFromBookData(const std::string& bookCode, int totalQuantity, int availableCount) {
        int idx = findByCode(bookCode);
        if (idx >= 0) {
            records[idx].quantity = totalQuantity;
            records[idx].available = availableCount;
            return;
        }
        records.emplace_back(bookCode, totalQuantity, availableCount);
    }

    void recordPurchase(const std::string& bookCode) {
        int idx = findByCode(bookCode);
        if (idx < 0) return;

        auto& rec = records[idx];

        if (rec.available <= 0) return;

        rec.purchasedOrBorrowed += 1;
        rec.available -= 1;
        rec.interestScore += 1;

        if (!rec.bonusApplied && rec.isOverHalfPurchased()) {
            rec.interestScore += 1;
            rec.bonusApplied = true;
        }
    }

    void recordReturn(const std::string& bookCode) {
        int idx = findByCode(bookCode);
        if (idx < 0) return;

        auto& rec = records[idx];
        if (rec.purchasedOrBorrowed > 0) {
            rec.purchasedOrBorrowed -= 1;
            rec.available += 1;
        }
    }

    int getInterestScore(const std::string& bookCode) const {
        int idx = findByCode(bookCode);
        return idx < 0 ? 0 : records[idx].interestScore;
    }

    std::vector<BookInterest> getRankedByInterest() const {
        auto result = records;
        std::sort(result.begin(), result.end(),
            [](const BookInterest& a, const BookInterest& b) {
                return a.interestScore > b.interestScore;
            });
        return result;
    }

    void syncFromJson(const std::string& bookCode, int newQuantity, int newAvailable) {
        int idx = findByCode(bookCode);
        if (idx >= 0) {
            records[idx].quantity = newQuantity;
            records[idx].available = newAvailable;
        }
    }
};

#endif 
