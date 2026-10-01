#ifndef BOOKINTEREST_H
#define BOOKINTEREST_H

#include <string>
#include <vector>
#include <algorithm>


struct BookInterest {
    std::string bookCode;
    int totalCopies = 0;       
    int borrowedCopies = 0;    
    int interestScore = 0;    
    bool bonusApplied = false; 
};
#endif // BOOKINTEREST_H
