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

class InterestManager {
private:
    std::vector<BookInterest> books;

    int findIndex(const std::string& code) const {
        for (size_t i = 0; i < books.size(); ++i)
            if (books[i].bookCode == code) return (int)i;
        return -1;
    }

    
    void recalcScore(BookInterest& b) {
        b.interestScore = b.borrowedCopies;
        if (b.borrowedCopies * 2 > b.totalCopies) {
            if (!b.bonusApplied) {
                b.interestScore += 1; // +1 thưởng
                b.bonusApplied = true;
            }
        } else {
            b.bonusApplied = false; 
        }
    }

public:
    
    void registerBook(const std::string& code, int totalCopies) {
        if (findIndex(code) >= 0) return;
        BookInterest b;
        b.bookCode = code;
        b.totalCopies = totalCopies;
        books.push_back(b);
    }

    
    void onBorrow(const std::string& code) {
        int idx = findIndex(code);
        if (idx < 0) return;
        BookInterest& b = books[idx];
        if (b.borrowedCopies >= b.totalCopies) return; // Hết sách
        b.borrowedCopies += 1;
        recalcScore(b);
    }

    
    void onReturn(const std::string& code) {
        int idx = findIndex(code);
        if (idx < 0) return;
        BookInterest& b = books[idx];
        if (b.borrowedCopies > 0) b.borrowedCopies -= 1;
        recalcScore(b);
    }

   
    int getScore(const std::string& code) const {
        int idx = findIndex(code);
        return idx < 0 ? 0 : books[idx].interestScore;
    }

    
    std::vector<BookInterest> getRanked() const {
        std::vector<BookInterest> res = books;
        std::sort(res.begin(), res.end(),
            [](const BookInterest& a, const BookInterest& b) {
                return a.interestScore > b.interestScore;
            });
        return res;
    }
};

#endif // BOOKINTEREST_H
