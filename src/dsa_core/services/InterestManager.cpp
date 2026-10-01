#include "InterestManager.h"
#include <algorithm>
#include <cctype>

InterestManager::InterestManager(BookRepository& bookRepository)
    : repo(bookRepository) {}

std::string InterestManager::toLower(const std::string& s) {
    std::string res;
    for (unsigned char c : s) {
        res += static_cast<char>(std::tolower(c));
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

std::vector<TrendingBook> InterestManager::getTrendingBooks() const {
    std::vector<TrendingBook> result;
    const std::vector<Book>& books = repo.getAll();

    for (const Book& book : books) {
        int total = static_cast<int>(book.copies.size());

        // Sach khong co ban nao thi khong the tinh ty le ban da muon.
        if (total <= 0) continue;

        int borrowed = countBorrowed(book);

        // =====================================================
        // NGHIEP VU "SACH DANG DUOC QUAN TAM" - NGUON DUY NHAT
        // Chi nhung sach co so ban dang duoc muon > 50% tong so ban
        // moi duoc coi la sach dang duoc quan tam.
        // Dung 50% (borrowed * 2 == total) KHONG duoc tinh.
        // Duoi hoang bang 50% cung khong duoc tinh.
        // =====================================================
        if (borrowed * 2 <= total) continue;

        int score = calculateScore(book);

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

    std::sort(result.begin(), result.end(),
        [](const TrendingBook& a, const TrendingBook& b) {
            return a.interestScore > b.interestScore;
        });
    return result;
}
