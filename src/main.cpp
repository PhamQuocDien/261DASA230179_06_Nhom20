#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>

#include "dsa_core/models/Book.h"
#include "dsa_core/models/Member.h"
#include "dsa_core/models/Date.h"
#include "dsa_core/models/Loan.h"
#include "dsa_core/models/Fine.h"
#include "dsa_core/models/Reservation.h"
#include "dsa_core/models/BookInterest.h"

#include "dsa_core/repositories/BookRepository.h"
#include "dsa_core/repositories/MemberRepository.h"
#include "dsa_core/repositories/LoanRepository.h"
#include "dsa_core/repositories/FineRepository.h"
#include "dsa_core/repositories/ReservationRepository.h"

#include "dsa_core/services/BookService.h"
#include "dsa_core/services/MemberService.h"
#include "dsa_core/services/LoanService.h"
#include "dsa_core/services/LoanSlipService.h"
#include "dsa_core/services/ReservationService.h"
#include "dsa_core/services/RenewService.h"
#include "dsa_core/services/InterestManager.h"
#include "dsa_core/services/AdminService.h"
#include "persistence/JsonDatabase.h"
#include "persistence/JsonMapper.h"
#include "dsa_core/structures/AVLTree.h"

using namespace std;
using nlohmann::json;


// =====================================================
// LẤY NGÀY HIỆN TẠI: YYYY-MM-DD
// =====================================================

string static getCurrentDateStr() {
    time_t now = time(nullptr);
    tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    return string(buffer);
}


// =====================================================
// BOOK -> JSON
// =====================================================

json static bookToJson(const Book& book) {
    return JsonMapper::bookToJson(book);
}


// =====================================================
// LOAN SLIP -> JSON
// =====================================================

json static loanSlipToJson(const LoanSlip& slip) {
    json result;

    result["loanId"] = slip.loanId;
    result["memberId"] = slip.memberId;
    result["memberName"] = slip.memberName;
    result["bookId"] = slip.bookId;
    result["bookCode"] = slip.bookCode;
    result["bookTitle"] = slip.bookTitle;
    result["borrowDate"] = slip.borrowDate;
    result["dueDate"] = slip.dueDate;

    if (slip.returnDate.empty()) {
        result["returnDate"] = nullptr;
    }
    else {
        result["returnDate"] = slip.returnDate;
    }

    result["renewalCount"] = slip.renewalCount;
    result["status"] = slip.status;

    return result;
}


// =====================================================
// SAVE BOOKS
// =====================================================

bool static saveBooksToDatabase(
    JsonDatabase& database,
    json& data,
    const BookRepository& bookRepository
) {
    data["books"] = json::array();

    const vector<Book>& books = bookRepository.getAll();

    for (const Book& book : books) {
        data["books"].push_back(
            JsonMapper::bookToJson(book)
        );
    }

    return database.save(data);
}


// =====================================================
// GENERATE NEXT MEMBER ID
// =====================================================

string static generateNextMemberId(
    const MemberRepository& memberRepository
) {
    unsigned long long maxId = 0;

    for (const Member& member : memberRepository.getAll()) {
        const string& id = member.memberId;

        if (id.size() < 2 ||
            (id[0] != 'M' && id[0] != 'm')) {
            continue;
        }

        unsigned long long value = 0;
        bool numeric = true;

        for (size_t i = 1; i < id.size(); ++i) {
            if (!isdigit(static_cast<unsigned char>(id[i]))) {
                numeric = false;
                break;
            }

            value =
                value * 10 +
                static_cast<unsigned long long>(id[i] - '0');
        }

        if (numeric && value > maxId) {
            maxId = value;
        }
    }

    string number = to_string(maxId + 1);

    while (number.size() < 3) {
        number = "0" + number;
    }

    return "M" + number;
}


// =====================================================
// SAVE MEMBERS
// =====================================================

bool static saveMembersToDatabase(
    JsonDatabase& database,
    json& data,
    const MemberRepository& memberRepository
) {
    data["members"] = json::array();

    for (const Member& member : memberRepository.getAll()) {
        data["members"].push_back(
            JsonMapper::memberToJson(member)
        );
    }

    return database.save(data);
}


// =====================================================
// SAVE LOAN + FINE + BOOK
// =====================================================

bool static saveLoanAndFineDataToDatabase(
    JsonDatabase& database,
    json& data,
    const BookRepository& bookRepository,
    const LoanRepository& loanRepository,
    const FineRepository& fineRepository
) {
    data["books"] = json::array();

    for (const Book& book : bookRepository.getAll()) {
        data["books"].push_back(
            JsonMapper::bookToJson(book)
        );
    }

    data["loans"] = json::array();

    for (const Loan& loan : loanRepository.getAll()) {
        data["loans"].push_back(
            JsonMapper::loanToJson(loan)
        );
    }

    data["fines"] = json::array();

    for (const Fine& fine : fineRepository.getAll()) {
        data["fines"].push_back(
            JsonMapper::fineToJson(fine)
        );
    }

    return database.save(data);
}


// =====================================================
// SAVE RESERVATIONS
// =====================================================

bool static saveReservationsToDatabase(
    JsonDatabase& database,
    json& data,
    const ReservationRepository& reservationRepository
) {
    data["reservations"] = json::array();

    for (const Reservation& reservation :
         reservationRepository.getAll()) {

        data["reservations"].push_back(
            JsonMapper::reservationToJson(reservation)
        );
    }

    return database.save(data);
}

// =====================================================
// API MODE
// =====================================================

int runApiMode(
    LoanSlipService& loanSlipService,
    MemberRepository& memberRepository,
    MemberService& memberService,
    BookService& bookService,
    BookRepository& bookRepository,
    LoanRepository& loanRepository,
    LoanService& loanService,
    FineRepository& fineRepository,
    ReservationService& reservationService,
    ReservationRepository& reservationRepository,
    RenewService& renewService,
    AdminService& adminService,
    JsonDatabase& database,
    json& data
) {
    json request;

    if (!(cin >> request)) {
        cout << json{
            {"success", false},
            {"error", "Khong doc duoc JSON Request."}
        }.dump();

        return 1;
    }

    if (!request.is_object()) {
        cout << json{
            {"success", false},
            {"error", "JSON Request phai la Object."}
        }.dump();

        return 1;
    }

    string action = request.value("action", "");


    // =================================================
    // LOGIN ADMIN
    // =================================================

    if (action == "loginAdmin") {

        string idAdmin =
            request.value("idAdmin", "");

        string password =
            request.value("password", "");

        bool authenticated =
            adminService.authenticate(
                idAdmin,
                password
            );

        if (!authenticated) {

            cout << json{
                {"success", false},
                {"error",
                "ID quan ly hoac mat khau khong dung."}
            }.dump();

            return 0;
        }

        cout << json{
            {"success", true},
            {"data",
            {
                {"authenticated", true}
            }}
        }.dump();

        return 0;
    }


    bool adminAuthorized =
        request.value("adminAuthorized", false);


// =================================================
// ADMIN ACTION
// =================================================
    if (action == "createBook" ||
        action == "updateBook" ||
        action == "addBookCopy" ||
        action == "deleteBook") {

        if (!adminAuthorized) {
            cout << json{
                {"success", false},
                {"error", "Khong co quyen quan ly."}
            }.dump();

            return 0;
        }
    }


    // =================================================
    // GET BOOKS
    // =================================================

    if (action == "getBooks") {

        const vector<Book>& books =
            bookService.getAllBooks();

        json bookData = json::array();

        for (const Book& book : books) {
            bookData.push_back(
                JsonMapper::bookToJson(book)
            );
        }

        json response = {
            {"success", true},
            {"data", bookData}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET AVAILABLE BOOKS
    // =================================================

    if (action == "getAvailableBooks") {

        const vector<Book> books =
            bookService.getAvailableBooks();

        json bookData = json::array();

        for (const Book& book : books) {

            json bookJson =
                JsonMapper::bookToJson(book);

            json bookIds =
                json::array();

            for (const BookCopy& bookCopy :
                 book.copies) {

                bookIds.push_back(bookCopy.bookId);
            }

            bookJson["bookIds"] = bookIds;

            bookJson["availableQuantity"] =
                static_cast<int>(book.copies.size());

            bookData.push_back(bookJson);
        }

        json response = {
            {"success", true},
            {"data", bookData}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET BORROWED BOOKS
    // =================================================

    if (action == "getBorrowedBooks") {

        const vector<Book> books =
            bookService.getBorrowedBooks();

        json bookData = json::array();

        for (const Book& book : books) {

            json bookJson =
                JsonMapper::bookToJson(book);

            json bookIds =
                json::array();

            for (const BookCopy& bookCopy :
                 book.copies) {

                bookIds.push_back(bookCopy.bookId);
            }

            bookJson["bookIds"] = bookIds;

            bookJson["borrowedQuantity"] =
                static_cast<int>(book.copies.size());

            bookData.push_back(bookJson);
        }

        json response = {
            {"success", true},
            {"data", bookData}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET BOOKS BY YEAR RANGE - MC2 (AVL TREE)
    // =================================================

    if (action == "getBooksByYearRange") {

        if (!request.contains("yearStart") ||
            !request.contains("yearEnd") ||
            !request["yearStart"].is_number_integer() ||
            !request["yearEnd"].is_number_integer()) {

            cout << json{
                {"success", false},
                {"error", "Thieu yearStart/yearEnd hoac gia tri khong hop le."}
            }.dump();

            return 1;
        }

        int yearStart = request["yearStart"].get<int>();
        int yearEnd = request["yearEnd"].get<int>();

        if (yearStart > yearEnd) {
            cout << json{
                {"success", false},
                {"error", "yearStart phai nho hon hoac bang yearEnd."}
            }.dump();

            return 0;
        }

        AVLTree avlTree;

        for (const Book& book : bookRepository.getAll()) {
            avlTree.insert(book);
        }

        vector<Book*> booksInRange =
            avlTree.findBookByYearRange(yearStart, yearEnd);

        json bookData = json::array();

        for (const Book* book : booksInRange) {
            if (book != nullptr) {
                bookData.push_back(
                    JsonMapper::bookToJson(*book)
                );
            }
        }

        json response = {
            {"success", true},
            {"data", bookData},
            {"yearStart", yearStart},
            {"yearEnd", yearEnd}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET ONE BOOK
    // =================================================

    if (action == "getBook") {

        string bookCode =
            request.value("bookCode", "");

        if (bookCode.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu bookCode."}
            }.dump();

            return 1;
        }

        Book* book =
            bookService.getBookByCode(bookCode);

        if (book == nullptr) {

            cout << json{
                {"success", false},
                {"error", "Book khong ton tai."}
            }.dump();

            return 0;
        }

        json response = {
            {"success", true},
            {"data", JsonMapper::bookToJson(*book)}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }
// =================================================
// SEARCH BOOK BY TITLE
// =================================================

if (action == "searchBookByTitle") {

    string keyword =
        request.value("keyword", "");

    if (keyword.empty()) {

        cout << json{
            {"success", false},
            {"error", "Thieu keyword."}
        }.dump();

        return 1;
    }

    vector<Book*> booksFound =
        bookRepository.findByTitle(keyword);

    json bookData =
        json::array();

    for (const Book* book : booksFound) {

        if (book != nullptr) {
            bookData.push_back(
                JsonMapper::bookToJson(*book)
            );
        }
    }

    json response = {
        {"success", true},
        {"data", bookData},
        {"keyword", keyword}
    };

    cout << response.dump(
        -1,
        ' ',
        false,
        json::error_handler_t::replace
    );

    return 0;
}

    // =================================================
    // REGISTER MEMBER
    // =================================================

    if (action == "registerMember") {

        string name =
            request.value("name", "");

        string email =
            request.value("email", "");

        string phone =
            request.value("phone", "");

        string password =
            request.value("password", "");


        auto trim = [](string value) {

            const string whitespace =
                " \t\r\n";

            size_t start =
                value.find_first_not_of(
                    whitespace
                );

            if (start == string::npos) {
                return string();
            }

            size_t end =
                value.find_last_not_of(
                    whitespace
                );

            return value.substr(
                start,
                end - start + 1
            );
        };


        name = trim(name);
        email = trim(email);
        phone = trim(phone);


        if (name.empty() ||
            email.empty() ||
            phone.empty()) {

            cout << json{
                {"success", false},
                {"error",
                 "Vui long nhap day du name, email va phone."}
            }.dump();

            return 0;
        }


        if (password.empty()) {

            cout << json{
                {"success", false},
                {"error",
                 "Vui long nhap mat khau thanh vien."}
            }.dump();

            return 0;
        }


        if (name.size() > 100 ||
            email.size() > 150 ||
            phone.size() > 30 ||
            password.size() > 100) {

            cout << json{
                {"success", false},
                {"error",
                 "Du lieu vuot qua do dai cho phep."}
            }.dump();

            return 0;
        }


        size_t atPos =
            email.find('@');

        size_t dotPos =
            email.find(
                '.',
                atPos == string::npos
                    ? 0
                    : atPos
            );


        if (atPos == string::npos ||
            atPos == 0 ||
            dotPos == string::npos ||
            dotPos <= atPos + 1 ||
            dotPos + 1 >= email.size()) {

            cout << json{
                {"success", false},
                {"error", "Email khong hop le."}
            }.dump();

            return 0;
        }


        for (char ch : phone) {

            if (!std::isdigit(
                    static_cast<unsigned char>(ch)
                ) &&
                ch != '+' &&
                ch != '-' &&
                ch != ' ' &&
                ch != '(' &&
                ch != ')') {

                cout << json{
                    {"success", false},
                    {"error",
                     "So dien thoai khong hop le."}
                }.dump();

                return 0;
            }
        }


        for (const Member& existing :
             memberRepository.getAll()) {

            if (existing.email == email) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Email da duoc dang ky."}
                }.dump();

                return 0;
            }

            if (existing.phone == phone) {

                cout << json{
                    {"success", false},
                    {"error",
                     "So dien thoai da duoc dang ky."}
                }.dump();

                return 0;
            }
        }


        Member member;

        member.name = name;
        member.email = email;
        member.phone = phone;
        member.password = password;
        member.status = "ACTIVE";


        if (!memberService.addMember(member)) {

            cout << json{
                {"success", false},
                {"error",
                 "Khong the tao thanh vien moi."}
            }.dump();

            return 0;
        }


        if (!saveMembersToDatabase(
                database,
                data,
                memberRepository
            )) {

            memberRepository.removeById(
                member.memberId
            );

            cout << json{
                {"success", false},
                {"error",
                 "Da tao thanh vien trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        cout << json{
            {"success", true},
            {"data",
             JsonMapper::memberToPublicJson(member)}
        }.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // CREATE BOOK
    // =================================================

    if (action == "createBook") {

        if (!request.contains("book") ||
            !request["book"].is_object()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu book hoac book khong hop le."}
            }.dump();

            return 1;
        }


        if (!request.contains("quantity") ||
            !request["quantity"].is_number_integer()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu quantity hoac quantity khong hop le."}
            }.dump();

            return 1;
        }


        int quantity =
            request["quantity"].get<int>();

        if (quantity <= 0) {

            cout << json{
                {"success", false},
                {"error",
                 "So luong cuon vat ly phai lon hon 0."}
            }.dump();

            return 0;
        }


        if (quantity > 999) {

            cout << json{
                {"success", false},
                {"error",
                 "So luong cuon vat ly khong duoc vuot qua 999."}
            }.dump();

            return 0;
        }


        try {

            Book book =
                JsonMapper::bookFromJson(
                    request["book"]
                );
            string enqueueDate = getCurrentDateStr();
            Book* book =
                bookService.getBookByCode(
                bookCode
            );
            if (book == nullptr) {
                cout << json{
                    {"success", false},
                    {"error", "Book khong ton tai."}
                }.dump();

               return 0;
            }
            bool hasAvailableCopy = false;
            for (const BookCopy& copy : book->copies) {
                if (copy.status == "AVAILABLE" ||
                    copy.status == "available") {
                    hasAvailableCopy = true;
                    break;
                }
            }
            if (hasAvailableCopy) {
            
                BorrowResult borrowResult =
                    loanService.borrowBook(
                        memberId,
                        bookCode,
                        enqueueDate
                    );
            
                if (!borrowResult.isSuccess) {
                    cout << json{
                        {"success", false},
                        {"error", borrowResult.message}
                    }.dump();
            
                    return 0;
                }
            
                bool loanSaved =
                    saveLoanAndFineDataToDatabase(
                        database,
                        data,
                        bookRepository,
                        loanRepository,
                        fineRepository
                    );
            
                if (!loanSaved) {
                    cout << json{
                        {"success", false},
                        {"error",
                         "Muon thanh cong nhung khong the luu library.json."}
                    }.dump();
            
                    return 1;
                }
            
                json response = {
                    {"success", true},
                    {"message", "Sach con ban co the muon, da tu dong cap sach."},
                    {"data", JsonMapper::loanToJson(borrowResult.loan)}
                };
            
                cout << response.dump(
                    -1,
                    ' ',
                    false,
                    json::error_handler_t::replace
                );
            
                return 0;
            }
            bool created =
                bookService.addBook(
                    book,
                    quantity
                );

            if (!created) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Khong the them Book. "
                     "Book co the bi trung "
                     "hoac du lieu khong hop le."}
                }.dump();

                return 0;
            }


            bool saved =
                saveBooksToDatabase(
                    database,
                    data,
                    bookRepository
                );

            if (!saved) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Book da duoc them vao bo nho "
                     "nhung khong the luu library.json."}
                }.dump();

                return 1;
            }


            Book* createdBook =
                bookService.getBookByCode(
                    book.bookCode
                );

            if (createdBook == nullptr) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Book da duoc tao nhung "
                     "khong the doc lai du lieu sau khi tao."}
                }.dump();

                return 1;
            }


            json response = {
                {"success", true},
                {"data",
                 JsonMapper::bookToJson(*createdBook)}
            };


            cout << response.dump(
                -1,
                ' ',
                false,
                json::error_handler_t::replace
            );

            return 0;
        }
        catch (const exception& e) {

            cout << json{
                {"success", false},
                {"error",
                 string("Du lieu Book khong hop le: ")
                 + e.what()}
            }.dump();

            return 1;
        }
    }


    // =================================================
    // UPDATE BOOK
    // =================================================

    if (action == "updateBook") {

        if (!request.contains("book") ||
            !request["book"].is_object()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu book hoac book khong hop le."}
            }.dump();

            return 1;
        }


        if (!request.contains("quantity") ||
            !request["quantity"].is_number_integer()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu quantity hoac quantity khong hop le."}
            }.dump();

            return 1;
        }


        int quantity =
            request["quantity"].get<int>();

        if (quantity <= 0) {

            cout << json{
                {"success", false},
                {"error",
                 "So luong cuon vat ly phai lon hon 0."}
            }.dump();

            return 0;
        }


        if (quantity > 999) {

            cout << json{
                {"success", false},
                {"error",
                 "So luong cuon vat ly khong duoc vuot qua 999."}
            }.dump();

            return 0;
        }


        try {

            Book book =
                JsonMapper::bookFromJson(
                    request["book"]
                );

            bool updated =
                bookService.updateBook(
                    book,
                    quantity
                );

            if (!updated) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Khong the cap nhat Book. "
                     "Book khong ton tai, "
                     "so luong moi nho hon so luong cu "
                     "hoac du lieu khong hop le."}
                }.dump();

                return 0;
            }


            bool saved =
                saveBooksToDatabase(
                    database,
                    data,
                    bookRepository
                );

            if (!saved) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Book da duoc cap nhat trong bo nho "
                     "nhung khong the luu library.json."}
                }.dump();

                return 1;
            }


            Book* updatedBook =
                bookService.getBookByCode(
                    book.bookCode
                );

            if (updatedBook == nullptr) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Book da duoc cap nhat "
                     "nhung khong the doc lai du lieu."}
                }.dump();

                return 1;
            }


            json response = {
                {"success", true},
                {"data",
                 JsonMapper::bookToJson(*updatedBook)}
            };


            cout << response.dump(
                -1,
                ' ',
                false,
                json::error_handler_t::replace
            );

            return 0;
        }
        catch (const exception& e) {

            cout << json{
                {"success", false},
                {"error",
                 string("Du lieu Book khong hop le: ")
                 + e.what()}
            }.dump();

            return 1;
        }
    }


    // =================================================
    // ADD BOOK COPY
    // =================================================

    if (action == "addBookCopy") {

        string bookCode =
            request.value("bookCode", "");

        if (bookCode.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu bookCode."}
            }.dump();

            return 1;
        }


        bool added =
            bookService.addBookCopy(bookCode);

        if (!added) {

            cout << json{
                {"success", false},
                {"error",
                 "Khong the them BookCopy. "
                 "Book khong ton tai "
                 "hoac khong the them."}
            }.dump();

            return 0;
        }


        bool saved =
            saveBooksToDatabase(
                database,
                data,
                bookRepository
            );

        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "BookCopy da duoc them vao bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        Book* updatedBook =
            bookService.getBookByCode(
                bookCode
            );

        if (updatedBook == nullptr) {

            cout << json{
                {"success", false},
                {"error",
                 "Book da duoc cap nhat "
                 "nhung khong the doc lai du lieu."}
            }.dump();

            return 1;
        }


        json response = {
            {"success", true},
            {"data",
             JsonMapper::bookToJson(*updatedBook)}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // DELETE BOOK
    // =================================================

    if (action == "deleteBook") {

        string bookCode =
            request.value("bookCode", "");

        string bookId =
            request.value("bookId", "");


        if (bookCode.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu bookCode."}
            }.dump();

            return 1;
        }


        bool deleted = false;


        if (bookId.empty()) {
            deleted =
                bookService.deleteBook(
                    bookCode
                );
        }
        else {
            deleted =
                bookService.deleteBookCopy(
                    bookCode,
                    bookId
                );
        }


        if (!deleted) {

            cout << json{
                {"success", false},
                {"error",
                 "Book khong ton tai, "
                 "Book_ID khong ton tai "
                 "hoac khong the xoa."}
            }.dump();

            return 0;
        }


        bool saved =
            saveBooksToDatabase(
                database,
                data,
                bookRepository
            );

        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "Du lieu da duoc xoa trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        json response = {
            {"success", true},
            {"data", {
                {"bookCode", bookCode},
                {"bookId", bookId},
                {"message",
                 bookId.empty()
                    ? "Book da duoc xoa."
                    : "BookCopy da duoc xoa."}
            }}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET LOAN SLIPS BY MEMBER
    // =================================================

    if (action == "getLoanSlipsByMember") {

        string memberId =
            request.value("memberId", "");

        if (memberId.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu memberId."}
            }.dump();

            return 1;
        }


        const Member* member =
            memberRepository.findById(
                memberId
            );

        if (member == nullptr) {

            cout << json{
                {"success", false},
                {"error",
                 "Member_ID khong ton tai."}
            }.dump();

            return 0;
        }


        vector<LoanSlip> slips =
            loanSlipService.getLoanSlipsByMember(
                memberId
            );

        json loanData =
            json::array();


        for (const LoanSlip& slip : slips) {

            loanData.push_back(
                loanSlipToJson(slip)
            );
        }


        json response = {
            {"success", true},
            {"data", loanData}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET LOAN SLIP BY LOAN ID
    // =================================================

    if (action == "getLoanSlipByLoanId") {

        string loanId =
            request.value("loanId", "");

        if (loanId.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu loanId."}
            }.dump();

            return 1;
        }


        LoanSlip slip;

        bool found =
            loanSlipService.getLoanSlipByLoanId(
                loanId,
                slip
            );


        if (!found) {

            cout << json{
                {"success", false},
                {"error",
                 "Loan_ID khong ton tai "
                 "hoac khong tao duoc phieu."}
            }.dump();

            return 0;
        }


        json response = {
            {"success", true},
            {"data",
             loanSlipToJson(slip)}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }
    
    if (action == "renewBook") {
        string loanId = request.value("loanId", "");
        string password = request.value("password", "");

        if (loanId.empty()) {
            cout << json{
                {"success", false},
                {"error", "Thieu loanId."}
            }.dump();
            return 1;
        }

        // =================================================
        // XAC THUC MAT KHAU THANH VIEN
        // =================================================

        string loanMemberId = "";

        for (const Loan& loan : loanRepository.getAll()) {
            if (loan.loanId == loanId) {
                loanMemberId = loan.memberId;
                break;
            }
        }

        if (loanMemberId.empty()) {
            cout << json{
                {"success", false},
                {"error", "Loan_ID khong ton tai."}
            }.dump();
            return 0;
        }

        MemberService::AuthResult renewAuth =
            memberService.authenticate(
                loanMemberId,
                password
            );

        if (!renewAuth.isSuccess) {
            cout << json{
                {"success", false},
                {"error", renewAuth.message}
            }.dump();
            return 0;
        }

        RenewResult result = renewService.renewBook(loanId);

        if (!result.isSuccess) {
            cout << json{
                {"success", false},
                {"error", result.message}
            }.dump();
            return 0;
        }

        bool saved = saveLoanAndFineDataToDatabase(
            database,
            data,
            bookRepository,
            loanRepository,
            fineRepository
        );

        if (!saved) {
            cout << json{
                {"success", false},
                {"error", "Gia han thanh cong nhung khong the luu library.json."}
            }.dump();
            return 1;
        }

        json response = {
            {"success", true},
            {"message", result.message},
            {"data", JsonMapper::loanToJson(result.loan)}
        };

        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }

    // =================================================
    // DELETE LOAN
    // =================================================

    if (action == "deleteLoan") {
        string loanId = request.value("loanId", "");
        string password = request.value("password", "");

        if (loanId.empty()) {
            cout << json{
                {"success", false},
                {"error", "Thieu loanId."}
            }.dump();
            return 1;
        }

        // LoanService chiu trach nhiem xac thuc
        // va kiem tra dieu kien xoa.
        DeleteLoanResult result =
            loanService.deleteLoan(
                loanId,
                password
            );

        if (!result.isSuccess) {
            cout << json{
                {"success", false},
                {"error", result.message}
            }.dump();
            return 0;
        }

        bool saved = saveLoanAndFineDataToDatabase(
            database,
            data,
            bookRepository,
            loanRepository,
            fineRepository
        );

        if (!saved) {
            cout << json{
                {"success", false},
                {"error", "Xoa phieu thanh cong nhung khong the luu library.json."}
            }.dump();
            return 1;
        }

        json response = {
            {"success", true},
            {"message", result.message},
            {"data", {{"loanId", result.loanId}}}
        };

        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    
    if (action == "borrowBook") {
        string memberId = request.value("memberId", "");
        string bookCode = request.value("bookCode", "");
        string password = request.value("password", "");
        string borrowDate = request.value("borrowDate", "");

        if (memberId.empty() || bookCode.empty()) {
            cout << json{
                {"success", false},
                {"error", "Thieu memberId hoac bookCode."}
            }.dump();
            return 1;
        }

        // =================================================
        // XAC THUC MAT KHAU THANH VIEN
        // =================================================

        MemberService::AuthResult borrowAuth =
            memberService.authenticate(
                memberId,
                password
            );

        if (!borrowAuth.isSuccess) {
            cout << json{
                {"success", false},
                {"error", borrowAuth.message}
            }.dump();
            return 0;
        }

        // Neu client khong gui ngay muon thi lay ngay hien tai
        if (borrowDate.empty()) {
            time_t now = time(nullptr);
            tm localTime{};
#ifdef _WIN32
            localtime_s(&localTime, &now);
#else
            localtime_r(&now, &localTime);
#endif
            char buffer[20];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
            borrowDate = string(buffer);
        }

        BorrowResult result = loanService.borrowBook(
            memberId,
            bookCode,
            borrowDate
        );

        if (!result.isSuccess) {
            cout << json{
                {"success", false},
                {"error", result.message}
            }.dump();
            return 0;
        }

        // =================================================
        // PHỤC VỤ HÀNG CHỜ NẾU VẪN CÒN SÁCH TRỐNG
        //
        // Thành viên không có Reservation vẫn mượn được
        // như mọi khi. Sau đó nếu đầu sách còn bản
        // AVAILABLE và đang có người chờ thì phục vụ
        // người đứng đầu hàng chờ ngay trong backend.
        // =================================================

        int servedCount =
            loanService.serveWaitingReservations(
                bookCode,
                borrowDate
            );

        // Luu lai thay doi cua Book va Loan xuong library.json
        bool saved = saveLoanAndFineDataToDatabase(
            database,
            data,
            bookRepository,
            loanRepository,
            fineRepository
        );

        if (!saved) {
            cout << json{
                {"success", false},
                {"error", "Muon thanh cong nhung khong the luu library.json."}
            }.dump();
            return 1;
        }

        // Luu Reservation vì có thể đã phục vụ lượt chờ
        bool reservationSaved =
            saveReservationsToDatabase(
                database,
                data,
                reservationRepository
            );

        if (!reservationSaved) {
            cout << json{
                {"success", false},
                {"error", "Da cap nhat trong bo nho "
                "nhung khong the luu Reservation vao library.json."}
            }.dump();
            return 1;
        }

        string borrowMessage = result.message;

        if (servedCount > 0) {
            borrowMessage +=
                " Da tu dong cap sach cho "
                + to_string(servedCount)
                + " thanh vien trong hang cho.";
        }

        json response = {
            {"success", true},
            {"message", borrowMessage},
            {"data", JsonMapper::loanToJson(result.loan)}
        };

        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    // =================================================
    // RETURN BOOK
    // =================================================

    // =================================================
// RETURN BOOK
// =================================================

if (action == "returnBook") {

    string loanId =
        request.value(
            "loanId",
            ""
        );

    string returnDateStr =
        request.value(
            "returnDate",
            ""
        );

    string quality =
        request.value(
            "quality",
            "Tot"
        );


    // ---------------------------------------------
    // KIEM TRA INPUT
    // ---------------------------------------------

    if (
        loanId.empty() ||
        returnDateStr.empty()
    ) {

        cout << json{
            {"success", false},
            {"error",
             "Thieu thong tin loanId "
             "hoac returnDate."}
        }.dump();

        return 1;
    }


    

    // ---------------------------------------------
    // PARSE RETURN DATE
    // ---------------------------------------------

    Date returnDate =
        Date::parse(
            returnDateStr
        );


    if (
        returnDate.year <= 0 ||
        returnDate.month <= 0 ||
        returnDate.day <= 0
    ) {

        cout << json{
            {"success", false},
            {"error",
             "returnDate khong hop le. "
             "Dinh dang dung: YYYY-MM-DD."}
        }.dump();

        return 0;
    }


    // ---------------------------------------------
    // THUC HIEN TRA SACH
    // ---------------------------------------------

    ReturnReceipt receipt =
        loanService.returnBook(
            loanId,
            returnDate,
            quality
        );


    // ---------------------------------------------
    // NEU TRA SACH THANH CONG
    // ---------------------------------------------

    if (receipt.isSuccess) {

      
        // -----------------------------------------
        // LUU BOOK + LOAN + FINE
        // -----------------------------------------

        bool saved =
            saveLoanAndFineDataToDatabase(
                database,
                data,
                bookRepository,
                loanRepository,
                fineRepository
            );


        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "Da cap nhat trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }

        // -----------------------------------------
        // LUU RESERVATION
        // -----------------------------------------

        bool reservationSaved =
            saveReservationsToDatabase(
                database,
                data,
                reservationRepository
            );

        if (!reservationSaved) {

            cout << json{
                {"success", false},
                {"error",
                 "Da tra sach thanh cong nhung "
                 "khong the luu Reservation vao library.json."}
            }.dump();

            return 1;
        }




        // -----------------------------------------
        // TAO RESPONSE
        // -----------------------------------------

        json responseData = {
            {"loanId", receipt.loanId},
            {"lateDays", receipt.lateDays},
            {"lateFee", receipt.lateFee},
            {"damageFee", receipt.damageFee},
            {"totalFee", receipt.totalFee}
        };




        // -----------------------------------------
        // RESPONSE
        // -----------------------------------------

        json response = {
            {"success", true},
            {"message", receipt.message},
            {"data", responseData}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // ---------------------------------------------
    // TRA SACH THAT BAI
    // ---------------------------------------------

    json response = {
        {"success", false},
        {"message", receipt.message},
        {"data", {
            {"loanId", receipt.loanId},
            {"lateDays", receipt.lateDays},
            {"lateFee", receipt.lateFee},
            {"damageFee", receipt.damageFee},
            {"totalFee", receipt.totalFee},
        }}
    };


    cout << response.dump(
        -1,
        ' ',
        false,
        json::error_handler_t::replace
    );

    return 0;
}

    // =================================================
    // GET FINE
    // =================================================

    if (action == "getFine") {

        string loanId =
            request.value("loanId", "");

        if (loanId.empty()) {

            cout << json{
                {"success", false},
                {"error",
                 "Vui long nhap Loan_ID."}
            }.dump();

            return 1;
        }


        Fine* fine =
            fineRepository.findByLoanId(
                loanId
            );


        if (fine == nullptr) {

            json response = {
                {"success", false},
                {"message",
                 "Khong tim thay thong tin tien phat "
                 "cho phieu muon nay."}
            };


            cout << response.dump(
                -1,
                ' ',
                false,
                json::error_handler_t::replace
            );

            return 0;
        }


        json response = {
            {"success", true},
            {"data", {
                {"fineId", fine->fineId},
                {"loanId", fine->loanId},
                {"memberId", fine->memberId},
                {"amount", fine->amount},
                {"reason", fine->reason},
                {"status", fine->status}
            }}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }

    // =================================================
    // PAY FINE (Admin xác nhận đã trả tiền)
    // =================================================

    if (action == "payFine") {

        string fineId =
            request.value("fineId", "");

        string idAdmin =
            request.value("idAdmin", "");

        string password =
            request.value("password", "");


        // Chuoi xac thuc do C++ kiem tra,
        // khong tin frontend.
        PayFineResult payResult =
            adminService.payFine(
                fineId,
                idAdmin,
                password
            );


        if (!payResult.isSuccess) {

            cout << json{
                {"success", false},
                {"error", payResult.message}
            }.dump();

            return 0;
        }


        bool saved =
            saveLoanAndFineDataToDatabase(
                database,
                data,
                bookRepository,
                loanRepository,
                fineRepository
            );


        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "Da cap nhat trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        json response = {
            {"success", true},
            {"message", payResult.message},
            {"data", {
                {"fineId", payResult.fineId},
                {"status", payResult.status}
            }}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }

// =================================================
// GET RESERVATIONS BY BOOK CODE
// =================================================

if (action == "getReservationsByBookCode") {

    string bookCode =
        request.value("bookCode", "");

    if (bookCode.empty()) {

        cout << json{
            {"success", false},
            {"error", "Thieu bookCode."}
        }.dump();

        return 1;
    }

    Book* book =
        bookService.getBookByCode(bookCode);

    if (book == nullptr) {

        cout << json{
            {"success", false},
            {"error", "Book khong ton tai."}
        }.dump();

        return 0;
    }

    json reservationData =
        json::array();

    for (const Reservation& reservation :
         reservationRepository.getAll()) {

        if (reservation.bookCode == bookCode &&
            reservation.status == "WAITING") {

            reservationData.push_back(
                JsonMapper::reservationToJson(
                    reservation
                )
            );
        }
    }

    json response = {
        {"success", true},
        {"data", reservationData}
    };

    cout << response.dump(
        -1,
        ' ',
        false,
        json::error_handler_t::replace
    );

    return 0;
}
    // =================================================
    // ENQUEUE RESERVATION
    // =================================================

    if (action == "enqueueReservation") {

        string memberId =
            request.value("memberId", "");

        string bookCode =
            request.value("bookCode", "");

        string password =
            request.value("password", "");


        if (memberId.empty() ||
            bookCode.empty()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu memberId hoac bookCode."}
            }.dump();

            return 1;
        }


        // =================================================
        // XAC THUC MAT KHAU THANH VIEN
        // =================================================

        MemberService::AuthResult reservationAuth =
            memberService.authenticate(
                memberId,
                password
            );

        if (!reservationAuth.isSuccess) {

            cout << json{
                {"success", false},
                {"error",
                 reservationAuth.message}
            }.dump();

            return 0;
        }


        bool created =
            reservationService.enqueue(
                memberId,
                bookCode
            );


        if (!created) {

            cout << json{
                {"success", false},
                {"error",
                 "Khong the tao yeu cau dat cho."}
            }.dump();

            return 0;
        }


        Reservation* reservation =
            reservationRepository.findByMemberAndBook(
                memberId,
                bookCode
            );


        if (reservation == nullptr) {

            cout << json{
                {"success", false},
                {"error",
                 "Da tao Reservation "
                 "nhung khong doc lai duoc du lieu."}
            }.dump();

            return 1;
        }


        string reservationId =
            reservation->reservationId;


        // =================================================
        // PHỤC VỤ NGAY NẾU ĐẦU SÁCH ĐANG CÓ SÁCH TRỐNG
        //
        // Đăng ký chờ khi vẫn còn bản AVAILABLE thì người
        // đứng đầu hàng chờ đủ điều kiện được cấp sách
        // ngay, không để Reservation nằm chờ trong khi sách
        // đã có sẵn.
        // =================================================

        string enqueueDate = getCurrentDateStr();

        int servedCount =
            loanService.serveWaitingReservations(
                bookCode,
                enqueueDate
            );

        if (servedCount > 0) {

            bool loanSaved =
                saveLoanAndFineDataToDatabase(
                    database,
                    data,
                    bookRepository,
                    loanRepository,
                    fineRepository
                );

            if (!loanSaved) {

                cout << json{
                    {"success", false},
                    {"error",
                     "Da cap sach trong bo nho "
                     "nhung khong the luu library.json."}
                }.dump();

                return 1;
            }
        }


        bool saved =
            saveReservationsToDatabase(
                database,
                data,
                reservationRepository
            );


        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "Da tao Reservation trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        // Đọc lại theo Reservation_ID sau khi phục vụ
        // hàng chờ để trả về đúng trạng thái hiện tại
        Reservation* servedReservation =
            reservationRepository.findById(
                reservationId
            );

        if (servedReservation == nullptr) {

            cout << json{
                {"success", false},
                {"error",
                 "Khong doc lai duoc Reservation."}
            }.dump();

            return 1;
        }


        json response = {
            {"success", true},
            {"data",
             JsonMapper::reservationToJson(
                 *servedReservation
             )}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // CANCEL RESERVATION
    // =================================================

    if (action == "cancelReservation") {

        string reservationId =
            request.value("reservationId", "");

        string password =
            request.value("password", "");


        if (reservationId.empty()) {

            cout << json{
                {"success", false},
                {"error",
                 "Thieu reservationId."}
            }.dump();

            return 1;
        }


        // =================================================
        // XAC THUC MAT KHAU VA HUY RESERVATION
        //
        // Chuoi xac thuc do ReservationService lay
        // tu du lieu Reservation, khong tin client.
        // =================================================

        CancelReservationResult cancelResult =
            reservationService.cancelWithPassword(
                reservationId,
                password
            );


        if (!cancelResult.isSuccess) {

            cout << json{
                {"success", false},
                {"error",
                 cancelResult.message}
            }.dump();

            return 0;
        }


        bool saved =
            saveReservationsToDatabase(
                database,
                data,
                reservationRepository
            );


        if (!saved) {

            cout << json{
                {"success", false},
                {"error",
                 "Da huy Reservation trong bo nho "
                 "nhung khong the luu library.json."}
            }.dump();

            return 1;
        }


        Reservation* reservation =
            reservationRepository.findById(
                reservationId
            );


        json response = {
            {"success", true},
            {"message", cancelResult.message},
            {"data",
             reservation == nullptr
                ? json{}
                : JsonMapper::reservationToJson(
                    *reservation
                )}
        };


        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // NEXT RESERVATION
    // =================================================

    if (action == "nextReservation") {

        string bookCode =
            request.value("bookCode", "");

        if (bookCode.empty()) {

            cout << json{
                {"success", false},
                {"error", "Thieu bookCode."}
            }.dump();

            return 1;
        }

        Reservation* reservation =
            reservationService.getNextEligible(
                bookCode
            );

        if (reservation == nullptr) {

            cout << json{
                {"success", false},
                {"error", "Khong co yeu cau WAITING nao."}
            }.dump();

            return 0;
        }

        json response = {
            {"success", true},
            {"data",
             JsonMapper::reservationToJson(
                 *reservation
             )}
        };

        cout << response.dump(
            -1,
            ' ',
            false,
            json::error_handler_t::replace
        );

        return 0;
    }


    // =================================================
    // GET TRENDING BOOKS (Sách được quan tâm)
    // =================================================
    if (action == "getTrendingBooks") {
        InterestManager interestManager(bookRepository);
        vector<TrendingBook> trending = interestManager.getTrendingBooks();

        json trendingData = json::array();
        for (const TrendingBook& tb : trending) {
            json item;
            item["bookCode"] = tb.bookCode;
            item["title"] = tb.title;
            item["author"] = tb.author;
            item["totalCopies"] = tb.totalCopies;
            item["borrowedCopies"] = tb.borrowedCopies;
            item["interestScore"] = tb.interestScore;
            item["bonusApplied"] = tb.bonusApplied;
            trendingData.push_back(item);
        }
        json response = {{"success", true}, {"data", trendingData}};
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }

    // =================================================
    // UNSUPPORTED ACTION   ← giữ nguyên cái này ở dưới
    // =================================================

    cout << json{
        {"success", false},
        {"error", "Action khong duoc ho tro."}
    }.dump();

    return 1;
}



// =====================================================
// MAIN
// =====================================================

int main(int argc, char* argv[]) {

    bool apiMode =
        argc > 1 &&
        string(argv[1]) == "--api";


    JsonDatabase database(
        "data/library.json"
    );


    json data =
        database.load();


    if (data.empty()) {

        if (apiMode) {

            cout << json{
                {"success", false},
                {"error",
                 "Khong load duoc library.json."}
            }.dump();

            return 1;
        }


        cout << "Khong load duoc library.json."
             << endl;

        return 1;
    }


    // =================================================
    // LOAD DATA FROM JSON
    // =================================================

    vector<Book> books =
        JsonMapper::booksFromJson(data);

    vector<Member> members =
        JsonMapper::membersFromJson(data);

    vector<Loan> loans =
        JsonMapper::loansFromJson(data);

    vector<Reservation> reservations =
        JsonMapper::reservationsFromJson(data);

    vector<Fine> fines =
        JsonMapper::finesFromJson(data);


    // =================================================
    // CREATE REPOSITORIES
    // =================================================

    BookRepository bookRepository;
    MemberRepository memberRepository;
    LoanRepository loanRepository;
    ReservationRepository reservationRepository;
    FineRepository fineRepository;


    // =================================================
    // LOAD DATA INTO MEMORY
    // =================================================

    bookRepository.getAll() =
        books;

    memberRepository.getAll() =
        members;

    loanRepository.getAll() =
        loans;

    reservationRepository.getAll() =
        reservations;

    fineRepository.getAll() =
        fines;


    // =================================================
    // CREATE SERVICES
    // =================================================

    BookService bookService(
        bookRepository
    );

    MemberService memberService(
        memberRepository
    );

    LoanSlipService loanSlipService(
        loanRepository,
        memberRepository,
        bookRepository
    );

    ReservationService reservationService(
        memberRepository,
        bookRepository,
        loanRepository,
        reservationRepository,
        fineRepository
    );

    LoanService loanService(
        loanRepository,
        bookRepository,
        fineRepository,
        memberRepository,
        reservationService
    );
    
    RenewService renewService(
        loanRepository,
        bookRepository,
        reservationRepository
    );
    AdminService adminService(fineRepository);
    // =================================================
    // API MODE
    // =================================================

    if (apiMode) {

        return runApiMode(
            loanSlipService,
            memberRepository,
            memberService,
            bookService,
            bookRepository,
            loanRepository,
            loanService,
            fineRepository,
            reservationService,
            reservationRepository,
            renewService,
            adminService,
            database,
            data
        );
    }
    else{
        cout<<"Chương trình không hỗ trợ chế độ console. Vui lòng sử dụng chế độ API."<<endl;
        return 1;
    } 
}
