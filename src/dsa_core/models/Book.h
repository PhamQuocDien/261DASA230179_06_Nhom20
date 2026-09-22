#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <vector>

struct BookCopy {
    std::string bookId;
    std::string status;
};
struct Book {
    std::string bookCode;
    std::string title;
    std::string author;
    std::string category;
    int year;
    std::vector<BookCopy> copies;
};

#endif
