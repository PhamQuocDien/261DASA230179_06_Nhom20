#ifndef BOOKINTEREST_H
#define BOOKINTEREST_H

#include <string>


struct BookInterest {
    std::string bookCode;   
    int quantity = 0;       
    int available = 0;      

    int purchasedOrBorrowed = 0; 
    int interestScore = 0;       
    bool bonusApplied = false;  
    BookInterest() = default;

    BookInterest(const std::string& code, int totalQty, int availQty)
        : bookCode(code), quantity(totalQty), available(availQty) {}

    bool isOverHalfPurchased() const {
        if (quantity <= 0) return false;
        return purchasedOrBorrowed * 2 > quantity;
    }
};

#endif 