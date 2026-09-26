#include "BookRepository.h"
using namespace std;
BookRepository::BookRepository() {}
vector<Book>& BookRepository::getAll() {
    return books;
}
const vector<Book>& BookRepository::getAll() const {
    return books;
}
Book* BookRepository::findByCode(const string& bookCode) {
    for (Book& book : books) {
        if (book.bookCode == bookCode) {
            return &book;
        }
    }
    return nullptr;
}
const Book* BookRepository::findByCode(const string& bookCode) const {
    for (const Book& book : books) {
        if (book.bookCode == bookCode) {
            return &book;
        }
    }
    return nullptr;
}
bool BookRepository::add(const Book& book) {
    if (findByCode(book.bookCode) != nullptr) {
        return false;
    }
    books.push_back(book);
    return true;
}
bool BookRepository::update(const Book& book) {
    Book* existingBook = findByCode(book.bookCode);
    if (existingBook == nullptr) {
        return false;
    }
    *existingBook = book;
    return true;
}
bool BookRepository::removeByCode(const string& bookCode) {
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->bookCode == bookCode) {
            books.erase(it);
            return true;
        }
    }
    return false;
}
vector<Book*> BookRepository::findByTitle(const string& keyword) {
    vector<Book*> result;

    if (keyword.empty()) {
        return result;
    }

    for (Book& book : books) {
        if (book.title.find(keyword) != string::npos) {
            result.push_back(&book);
        }
    }

    return result;
}
