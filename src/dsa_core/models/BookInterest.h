#ifndef BOOKINTEREST_H
#define BOOKINTEREST_H

#include <string>
#include <vector>


struct BookCopy {
    std::string bookId;
    std::string status; 
};

struct BookInterest {
    std::string bookCode;    
    std::string title;       
    double price = 0.0;      
    std::vector<BookCopy> copies; 

    int interestScore = 0;
    bool bonusApplied = false; 

    BookInterest() = default;

   
    int totalCopies() const { return static_cast<int>(copies.size()); }

    
    int borrowedCount() const {
        int cnt = 0;
        for (const auto& c : copies) {
            std::string s = c.status;
            for (char& ch : s) ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
            if (s == "borrowed") cnt++;
        }
        return cnt;
    }

    
    bool isOverHalfBorrowed() const {
        int total = totalCopies();
        if (total <= 0) return false;
        return borrowedCount() * 2 > total;
    }
};

#endif 
