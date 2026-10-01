#include "InterestManager.h"
#include <algorithm>
#include <cctype>

using namespace std;

InterestManager::InterestManager(BookRepository& repo)
    : bookRepository(repo) {}


string InterestManager::toLower(const string& s) {
    string res;
    for (unsigned char c : s) {
        res += static_cast<char>(tolower(c));
    }
    return res;
}


int InterestManager::countBorrowed(const Book& book) {
    int cnt = 0;
    for (const BookCopy& copy : book.copies) {
        if (toLower(copy.status) == "borrowed") {
            cnt++;
        }
    }
    return cnt;
}


int InterestManager::calculateScore(const Book& book) const {
    int total = static_cast<int>(book.copies.size());
    if (total <= 0) return 0;

    int borrowed = countBorrowed(book);
    int score = borrowed; 

    
    if (borrowed * 2 > total) {
        score += 1;
    }

    return score;
}


vector<TrendingBook> InterestManager::getTrendingBooks() const {
    vector<TrendingBook> result;
    const vector<Book>& books = bookRepository.getAll();

    for (const Book& book : books) {
        int total = static_cast<int>(book.copies.size());
        int borrowed = countBorrowed(book);
        int score = calculateScore(book);

        if (score <= 0) continue; 

        TrendingBook tb;
        tb.bookCode = book.bookCode;
        tb.title = book.title;
        tb.author = book.author;
        tb.totalCopies = total;
        tb.borrowedCopies = borrowed;
        tb.interestScore = score;
        tb.bonusApplied = (borrowed * 2 > total);
        result.push_back(tb);
    }

   
    sort(result.begin(), result.end(),
        [](const TrendingBook& a, const TrendingBook& b) {
            return a.interestScore > b.interestScore;
        });

    return result;
}
