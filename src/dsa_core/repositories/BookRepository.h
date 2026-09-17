#ifndef BOOKREPOSITORY_H
#define BOOKREPOSITORY_H
#include <string>
#include <vector>
#include "../models/Book.h"
class BookRepository {
private:
    std::vector<Book> books;
public:
    BookRepository();
    std::vector<Book>& getAll();
    const std::vector<Book>& getAll() const;
    Book* findByCode(const std::string& bookCode);
    const Book* findByCode(const std::string& bookCode) const;
    bool add(const Book& book);
    bool update(const Book& book);
    bool removeByCode(const std::string& bookCode);
};
#endif