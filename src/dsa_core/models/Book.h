#ifndef BOOK_H
#define BOOK_H
#include <string>
#include <vector>
using namespace std;
struct BookCopy {
    string bookId;
    string status;
};
struct Book {
    string bookCode;
    string title;
    string author;
    string category;
    int year;
    vector<BookCopy> copies;
};

#endif