#ifndef INTERESTMANAGER_H
#define INTERESTMANAGER_H

#include <string>
#include <vector>
#include "../models/Book.h"          
#include "../repositories/BookRepository.h"


struct TrendingBook {
    std::string bookCode;
    std::string title;
    std::string author;
    int totalCopies;      
    int borrowedCopies;   
    int interestScore;     
    bool bonusApplied;     
};

class InterestManager {
private:
    BookRepository& bookRepository;

   
    static std::string toLower(const std::string& s);

    
    static int countBorrowed(const Book& book);

public:
    explicit InterestManager(BookRepository& repo);

    
    int calculateScore(const Book& book) const;

   
    std::vector<TrendingBook> getTrendingBooks() const;
};

#endif 
