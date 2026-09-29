#include "InterestManager.h"
#include <algorithm>

int InterestManager::findByCode(const std::string& bookCode) const {
    for (size_t i = 0; i < records.size(); ++i) {
        if (records[i].bookCode == bookCode) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

void InterestManager::loadFromBookData(const std::string& bookCode, int totalQuantity, int availableCount) {
    int idx = findByCode(bookCode);
    if (idx >= 0) {
        records[idx].quantity = totalQuantity;
        records[idx].available = availableCount;
        return;
    }
    records.emplace_back(bookCode, totalQuantity, availableCount);
}

void InterestManager::recordPurchase(const std::string& bookCode) {
    int idx = findByCode(bookCode);
    if (idx < 0) return;

    BookInterest& rec = records[idx];
    if (rec.available <= 0) return; 

    rec.purchasedOrBorrowed += 1;
    rec.available -= 1;
    rec.interestScore += 1;

    if (!rec.bonusApplied && rec.isOverHalfPurchased()) {
        rec.interestScore += 1;
        rec.bonusApplied = true;
    }
}


void InterestManager::recordReturn(const std::string& bookCode) {
    int idx = findByCode(bookCode);
    if (idx < 0) return;

    BookInterest& rec = records[idx];
    if (rec.purchasedOrBorrowed > 0) {
        rec.purchasedOrBorrowed -= 1;
        rec.available += 1;
    }
}

int InterestManager::getInterestScore(const std::string& bookCode) const {
    int idx = findByCode(bookCode);
    return (idx < 0) ? 0 : records[idx].interestScore;
}

std::vector<BookInterest> InterestManager::getRankedByInterest() const {
    std::vector<BookInterest> result = records;
    std::sort(result.begin(), result.end(),
        [](const BookInterest& a, const BookInterest& b) {
            return a.interestScore > b.interestScore;
        });
    return result;
}

void InterestManager::syncFromJson(const std::string& bookCode, int newQuantity, int newAvailable) {
    int idx = findByCode(bookCode);
    if (idx >= 0) {
        records[idx].quantity = newQuantity;
        records[idx].available = newAvailable;
    }
}
