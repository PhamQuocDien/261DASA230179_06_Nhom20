#ifndef BOOKSERVICE_H
#define BOOKSERVICE_H
#include <string>
#include <vector>
#include "../models/Book.h"
#include "../repositories/BookRepository.h"
class BookService {
private:
    BookRepository& repository;
public:
    explicit BookService(BookRepository& repository);
    bool addBook(const Book& book, int quantity);
    const std::vector<Book>& getAllBooks() const;
    Book* getBookByCode(const std::string& bookCode);
    bool updateBook(const Book& book, int quantity);
    bool addBookCopy(const std::string& bookCode);
    bool deleteBook(const std::string& bookCode);
    bool deleteBookCopy(const std::string& bookCode, const std::string& bookId);
};
#endif