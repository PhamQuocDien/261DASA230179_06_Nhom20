#ifndef BOOK_H
#define BOOK_H

#include <iostream>
#include <string>

using namespace std;

class Book {
public:

    // Mã đầu sách
    string BookCode;

    // ID duy nhất của bản sách
    string Book_ID;

    string Title;
    string Author;
    string Category;

    int Year;
    int Quantity;

    // Số lượt được quan tâm
    int interestCount;


    Book() {
        Year = 0;
        Quantity = 0;
        interestCount = 0;
    }


    Book(
        string code,
        string id,
        string title,
        string author,
        string category,
        int year,
        int quantity
    ) {

        BookCode = code;
        Book_ID = id;

        Title = title;
        Author = author;
        Category = category;

        Year = year;
        Quantity = quantity;

        interestCount = 0;
    }


    void display() const {

        cout << "Ma sach       : " << BookCode << endl;
        cout << "Book_ID       : " << Book_ID << endl;
        cout << "Ten sach      : " << Title << endl;
        cout << "Tac gia       : " << Author << endl;
        cout << "The loai      : " << Category << endl;
        cout << "Nam xuat ban  : " << Year << endl;
        cout << "So luong      : " << Quantity << endl;
        cout << "Luot quan tam : " << interestCount << endl;
    }
};

#endif

