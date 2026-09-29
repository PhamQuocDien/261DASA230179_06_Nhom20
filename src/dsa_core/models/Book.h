#ifndef BOOK_H
#define BOOK_H

#include <string>

struct Book {
    std::string bookCode;
    std::string title;
    std::string author;
    std::string category;
    int year = 0;
    int quantity = 0;
    int available = 0;

    Book() = default;
    Book(std::string code, std::string t, std::string a, std::string cat, int y, int qty)
        : bookCode(std::move(code)), title(std::move(t)), author(std::move(a)),
          category(std::move(cat)), year(y), quantity(qty), available(qty) {}
};

// Hằng số trạng thái — chuẩn hóa 1 nơi duy nhất
inline const std::string STATUS_AVAILABLE  = "AVAILABLE";
inline const std::string STATUS_BORROWED   = "BORROWED";
inline const std::string STATUS_DAMAGED    = "DAMAGED";

#endif // BOOK_H

