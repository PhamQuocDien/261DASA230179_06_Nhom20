
#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>

using namespace std;

class Book {
public:
    // Mã đầu sách
    string bookCode;

    // ID duy nhất của bản sách
    string bookId;

    // Thông tin sách
    string title;
    string author;
    string category;

    // Năm xuất bản
    int year;

    // Số lượng bản sách
    int quantity;

    // Số lượt được quan tâm
    int interestCount;

    // Constructor mặc định
    Book()
        : bookCode(""),
          bookId(""),
          title(""),
          author(""),
          category(""),
          year(0),
          quantity(0),
          interestCount(0)
    {
    }

    // Constructor đầy đủ
    Book(
        string code,
        string id,
        string bookTitle,
        string bookAuthor,
        string bookCategory,
        int bookYear,
        int bookQuantity
    )
        : bookCode(code),
          bookId(id),
          title(bookTitle),
          author(bookAuthor),
          category(bookCategory),
          year(bookYear),
          quantity(bookQuantity),
          interestCount(0)
    {
    }

    // Hiển thị thông tin
    void display() const {
        cout << "Book Code     : " << bookCode << endl;
        cout << "Book ID       : " << bookId << endl;
        cout << "Title         : " << title << endl;
        cout << "Author        : " << author << endl;
        cout << "Category      : " << category << endl;
        cout << "Year          : " << year << endl;
        cout << "Quantity      : " << quantity << endl;
        cout << "Interest      : " << interestCount << endl;
    }
};

#endif



