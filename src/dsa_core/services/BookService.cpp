#include "BookService.h"
#include <iomanip>
#include <sstream>
using namespace std;
BookService::BookService(BookRepository& repository) : repository(repository) {}
bool BookService::addBook(const Book& book, int quantity) {
    if (book.bookCode.empty()) {
        return false;
    }
    if (book.title.empty()) {
        return false;
    }
    if (book.year < 0) {
        return false;
    }
    if (quantity <= 0) {
        return false;
    }
    if (quantity > 999) {
        return false;
    }
    if (repository.findByCode(book.bookCode) != nullptr) {
        return false;
    }
    Book newBook = book;
    newBook.copies.clear();
    for (int i = 1; i <= quantity; i++) {
        ostringstream stream;
        stream << book.bookCode << setw(3) << setfill('0') << i;
        BookCopy bookCopy;
        bookCopy.bookId = stream.str();
        bookCopy.status = "available";
        newBook.copies.push_back(bookCopy);
    }
    return repository.add(newBook);
}
const vector<Book>& BookService::getAllBooks() const {
    return repository.getAll();
}
Book* BookService::getBookByCode(const string& bookCode) {
    if (bookCode.empty()) {
        return nullptr;
    }
    return repository.findByCode(bookCode);
}
bool BookService::updateBook(const Book& book) {
    if (book.bookCode.empty()) {
        return false;
    }
    if (book.title.empty()) {
        return false;
    }
    if (book.year < 0) {
        return false;
    }
    if (repository.findByCode(book.bookCode) == nullptr) {
        return false;
    }
    return repository.update(book);
}
bool BookService::deleteBook(const string& bookCode) {
    if (bookCode.empty()) {
        return false;
    }
    return repository.removeByCode(bookCode);
}