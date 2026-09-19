#include <iostream>
#include <string>
#include <vector>
#include "dsa_core/models/Book.h"
#include "dsa_core/models/Member.h"
#include "dsa_core/models/Date.h"
#include "dsa_core/models/Loan.h"
#include "dsa_core/models/Fine.h"
#include "dsa_core/repositories/BookRepository.h"
#include "dsa_core/repositories/MemberRepository.h"
#include "dsa_core/repositories/LoanRepository.h"
#include "dsa_core/repositories/FineRepository.h"
#include "dsa_core/services/BookService.h"
#include "dsa_core/services/LoanService.h"
#include "dsa_core/services/LoanSlipService.h"
#include "persistence/JsonDatabase.h"
#include "persistence/JsonMapper.h"
using namespace std;
using nlohmann::json;
json bookToJson(const Book& book) {
    return JsonMapper::bookToJson(book);
}
json loanSlipToJson(const LoanSlip& slip) {
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
bool saveBooksToDatabase(JsonDatabase& database, json& data, const BookRepository& bookRepository) {
    data["books"] = json::array();
    const vector<Book>& books = bookRepository.getAll();
    for (const Book& book : books) {
        data["books"].push_back(JsonMapper::bookToJson(book));
    }
    return database.save(data);
}
bool saveLoanAndFineDataToDatabase(JsonDatabase& database, json& data, const BookRepository& bookRepository, const LoanRepository& loanRepository, const FineRepository& fineRepository) {
    data["books"] = json::array();
    for (const Book& book : bookRepository.getAll()) {
        data["books"].push_back(JsonMapper::bookToJson(book));
    }
    data["loans"] = json::array();
    for (const Loan& loan : loanRepository.getAll()) {
        data["loans"].push_back(JsonMapper::loanToJson(loan));
    }
    data["fines"] = json::array();
    for (const Fine& fine : fineRepository.getAll()) {
        data["fines"].push_back(JsonMapper::fineToJson(fine));
    }
    return database.save(data);
}
void printLoanSlip(const LoanSlip& slip) {
    cout << endl;
    cout << "========================================" << endl;
    cout << "            PHIEU MUON SACH             " << endl;
    cout << "========================================" << endl;
    cout << "Ma phieu:       " << slip.loanId << endl;
    cout << "Ma thanh vien:  " << slip.memberId << endl;
    cout << "Thanh vien:     " << slip.memberName << endl;
    cout << "Ma ban sach:    " << slip.bookId << endl;
    cout << "Ma dau sach:    " << slip.bookCode << endl;
    cout << "Ten sach:       " << slip.bookTitle << endl;
    cout << "Ngay muon:      " << slip.borrowDate << endl;
    cout << "Han tra:        " << slip.dueDate << endl;
    cout << "Ngay tra:       ";
    if (slip.returnDate.empty()) {
        cout << "Chua tra";
    }
    else {
        cout << slip.returnDate;
    }
    cout << endl;
    cout << "So lan gia han: " << slip.renewalCount << endl;
    cout << "Trang thai:     " << slip.status << endl;
    cout << "========================================" << endl;
}
void printLoanSlips(const vector<LoanSlip>& slips) {
    if (slips.empty()) {
        cout << endl;
        cout << "Khong co phieu muon nao." << endl;
        return;
    }
    cout << endl;
    cout << "Tim thay " << slips.size() << " phieu muon." << endl;
    for (const LoanSlip& slip : slips) {
        printLoanSlip(slip);
    }
}
int runApiMode(LoanSlipService& loanSlipService, MemberRepository& memberRepository, BookService& bookService, BookRepository& bookRepository, LoanRepository& loanRepository, LoanService& loanService, FineRepository& fineRepository, JsonDatabase& database, json& data) {
    json request;
    if (!(cin >> request)) {
        cout << json{ {"success", false}, {"error", "Khong doc duoc JSON Request."} }.dump();
        return 1;
    }
    if (!request.is_object()) {
        cout << json{ {"success", false}, {"error", "JSON Request phai la Object."} }.dump();
        return 1;
    }
    string action = request.value("action", "");
    if (action == "getBooks") {
        const vector<Book>& books = bookService.getAllBooks();
        json bookData = json::array();
        for (const Book& book : books) {
            bookData.push_back(JsonMapper::bookToJson(book));
        }
        json response = { {"success", true}, {"data", bookData} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "getBook") {
        string bookCode = request.value("bookCode", "");
        if (bookCode.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu bookCode."} }.dump();
            return 1;
        }
        Book* book = bookService.getBookByCode(bookCode);
        if (book == nullptr) {
            cout << json{ {"success", false}, {"error", "Book khong ton tai."} }.dump();
            return 0;
        }
        json response = { {"success", true}, {"data", JsonMapper::bookToJson(*book)} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "createBook") {
        if (!request.contains("book") || !request["book"].is_object()) {
            cout << json{ {"success", false}, {"error", "Thieu book hoac book khong hop le."} }.dump();
            return 1;
        }
        if (!request.contains("quantity") || !request["quantity"].is_number_integer()) {
            cout << json{ {"success", false}, {"error", "Thieu quantity hoac quantity khong hop le."} }.dump();
            return 1;
        }
        int quantity = request["quantity"].get<int>();
        if (quantity <= 0) {
            cout << json{ {"success", false}, {"error", "So luong cuon vat ly phai lon hon 0."} }.dump();
            return 0;
        }
        if (quantity > 999) {
            cout << json{ {"success", false}, {"error", "So luong cuon vat ly khong duoc vuot qua 999."} }.dump();
            return 0;
        }
        try {
            Book book = JsonMapper::bookFromJson(request["book"]);
            bool created = bookService.addBook(book, quantity);
            if (!created) {
                cout << json{ {"success", false}, {"error", "Khong the them Book. Book co the bi trung hoac du lieu khong hop le."} }.dump();
                return 0;
            }
            bool saved = saveBooksToDatabase(database, data, bookRepository);
            if (!saved) {
                cout << json{ {"success", false}, {"error", "Book da duoc them vao bo nho nhung khong the luu library.json."} }.dump();
                return 1;
            }
            Book* createdBook = bookService.getBookByCode(book.bookCode);
            if (createdBook == nullptr) {
                cout << json{ {"success", false}, {"error", "Book da duoc tao nhung khong the doc lai du lieu sau khi tao."} }.dump();
                return 1;
            }
            json response = { {"success", true}, {"data", JsonMapper::bookToJson(*createdBook)} };
            cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
            return 0;
        }
        catch (const exception& e) {
            cout << json{ {"success", false}, {"error", string("Du lieu Book khong hop le: ") + e.what()} }.dump();
            return 1;
        }
    }
    if (action == "updateBook") {
        if (!request.contains("book") || !request["book"].is_object()) {
            cout << json{ {"success", false}, {"error", "Thieu book hoac book khong hop le."} }.dump();
            return 1;
        }
        if (!request.contains("quantity") || !request["quantity"].is_number_integer()) {
            cout << json{ {"success", false}, {"error", "Thieu quantity hoac quantity khong hop le."} }.dump();
            return 1;
        }
        int quantity = request["quantity"].get<int>();
        if (quantity <= 0) {
            cout << json{ {"success", false}, {"error", "So luong cuon vat ly phai lon hon 0."} }.dump();
            return 0;
        }
        if (quantity > 999) {
            cout << json{ {"success", false}, {"error", "So luong cuon vat ly khong duoc vuot qua 999."} }.dump();
            return 0;
        }
        try {
            Book book = JsonMapper::bookFromJson(request["book"]);
            bool updated = bookService.updateBook(book, quantity);
            if (!updated) {
                cout << json{ {"success", false}, {"error", "Khong the cap nhat Book. Book khong ton tai, so luong moi nho hon so luong cu hoac du lieu khong hop le."} }.dump();
                return 0;
            }
            bool saved = saveBooksToDatabase(database, data, bookRepository);
            if (!saved) {
                cout << json{ {"success", false}, {"error", "Book da duoc cap nhat trong bo nho nhung khong the luu library.json."} }.dump();
                return 1;
            }
            Book* updatedBook = bookService.getBookByCode(book.bookCode);
            if (updatedBook == nullptr) {
                cout << json{ {"success", false}, {"error", "Book da duoc cap nhat nhung khong the doc lai du lieu."} }.dump();
                return 1;
            }
            json response = { {"success", true}, {"data", JsonMapper::bookToJson(*updatedBook)} };
            cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
            return 0;
        }
        catch (const exception& e) {
            cout << json{ {"success", false}, {"error", string("Du lieu Book khong hop le: ") + e.what()} }.dump();
            return 1;
        }
    }
    if (action == "addBookCopy") {
        string bookCode = request.value("bookCode", "");
        if (bookCode.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu bookCode."} }.dump();
            return 1;
        }
        bool added = bookService.addBookCopy(bookCode);
        if (!added) {
            cout << json{ {"success", false}, {"error", "Khong the them BookCopy. Book khong ton tai hoac khong the them."} }.dump();
            return 0;
        }
        bool saved = saveBooksToDatabase(database, data, bookRepository);
        if (!saved) {
            cout << json{ {"success", false}, {"error", "BookCopy da duoc them vao bo nho nhung khong the luu library.json."} }.dump();
            return 1;
        }
        Book* updatedBook = bookService.getBookByCode(bookCode);
        if (updatedBook == nullptr) {
            cout << json{ {"success", false}, {"error", "Book da duoc cap nhat nhung khong the doc lai du lieu."} }.dump();
            return 1;
        }
        json response = { {"success", true}, {"data", JsonMapper::bookToJson(*updatedBook)} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "deleteBook") {
        string bookCode = request.value("bookCode", "");
        string bookId = request.value("bookId", "");
        if (bookCode.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu bookCode."} }.dump();
            return 1;
        }
        bool deleted = false;
        if (bookId.empty()) {
            deleted = bookService.deleteBook(bookCode);
        }
        else {
            deleted = bookService.deleteBookCopy(bookCode, bookId);
        }
        if (!deleted) {
            cout << json{ {"success", false}, {"error", "Book khong ton tai, Book_ID khong ton tai hoac khong the xoa."} }.dump();
            return 0;
        }
        bool saved = saveBooksToDatabase(database, data, bookRepository);
        if (!saved) {
            cout << json{ {"success", false}, {"error", "Du lieu da duoc xoa trong bo nho nhung khong the luu library.json."} }.dump();
            return 1;
        }
        json response = { {"success", true}, {"data", {{"bookCode", bookCode}, {"bookId", bookId}, {"message", bookId.empty() ? "Book da duoc xoa." : "BookCopy da duoc xoa."}}} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "getLoanSlipsByMember") {
        string memberId = request.value("memberId", "");
        if (memberId.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu memberId."} }.dump();
            return 1;
        }
        const Member* member = memberRepository.findById(memberId);
        if (member == nullptr) {
            cout << json{ {"success", false}, {"error", "Member_ID khong ton tai."} }.dump();
            return 0;
        }
        vector<LoanSlip> slips = loanSlipService.getLoanSlipsByMember(memberId);
        json loanData = json::array();
        for (const LoanSlip& slip : slips) {
            loanData.push_back(loanSlipToJson(slip));
        }
        json response = { {"success", true}, {"data", loanData} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "getLoanSlipByLoanId") {
        string loanId = request.value("loanId", "");
        if (loanId.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu loanId."} }.dump();
            return 1;
        }
        LoanSlip slip;
        bool found = loanSlipService.getLoanSlipByLoanId(loanId, slip);
        if (!found) {
            cout << json{ {"success", false}, {"error", "Loan_ID khong ton tai hoac khong tao duoc phieu."} }.dump();
            return 0;
        }
        json response = { {"success", true}, {"data", loanSlipToJson(slip)} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "returnBook") {
        string loanId = request.value("loanId", "");
        string returnDateStr = request.value("returnDate", "");
        string quality = request.value("quality", "Tot");
        if (loanId.empty() || returnDateStr.empty()) {
            cout << json{ {"success", false}, {"error", "Thieu thong tin loanId hoac returnDate."} }.dump();
            return 1;
        }
        Date returnDate = Date::parse(returnDateStr);
        if (returnDate.year <= 0 || returnDate.month <= 0 || returnDate.day <= 0) {
            cout << json{ {"success", false}, {"error", "returnDate khong hop le. Dinh dang dung: YYYY-MM-DD."} }.dump();
            return 0;
        }
        ReturnReceipt receipt = loanService.returnBook(loanId, returnDate, quality);
        if (receipt.isSuccess) {
            bool saved = saveLoanAndFineDataToDatabase(database, data, bookRepository, loanRepository, fineRepository);
            if (!saved) {
                cout << json{ {"success", false}, {"error", "Da cap nhat trong bo nho nhung khong the luu library.json."} }.dump();
                return 1;
            }
        }
        json response = { {"success", receipt.isSuccess}, {"message", receipt.message}, {"data", {{"loanId", receipt.loanId}, {"lateDays", receipt.lateDays}, {"lateFee", receipt.lateFee}, {"damageFee", receipt.damageFee}, {"totalFee", receipt.totalFee}}} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    if (action == "getFine") {
        string loanId = request.value("loanId", "");
        if (loanId.empty()) {
            cout << json{ {"success", false}, {"error", "Vui long nhap Loan_ID."} }.dump();
            return 1;
        }
        Fine* fine = fineRepository.findByLoanId(loanId);
        if (fine == nullptr) {
            json response = { {"success", false}, {"message", "Khong tim thay thong tin tien phat cho phieu muon nay."} };
            cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
            return 0;
        }
        json response = { {"success", true}, {"data", {{"fineId", fine->fineId}, {"loanId", fine->loanId}, {"memberId", fine->memberId}, {"amount", fine->amount}, {"reason", fine->reason}, {"status", fine->status}}} };
        cout << response.dump(-1, ' ', false, json::error_handler_t::replace);
        return 0;
    }
    cout << json{ {"success", false}, {"error", "Action khong duoc ho tro."} }.dump();
    return 1;
}
int runConsoleMode(LoanSlipService& loanSlipService, BookService& bookService) {
    cout << "========================================" << endl;
    cout << "       TRA CUU PHIEU MUON SACH          " << endl;
    cout << "========================================" << endl;
    while (true) {
        cout << endl;
        cout << "----------------------------------------" << endl;
        cout << "Nhap Member_ID (hoac Q de thoat): ";
        string memberId;
        if (!(cin >> memberId)) {
            break;
        }
        if (memberId == "Q" || memberId == "q") {
            cout << endl;
            cout << "Ket thuc chuong trinh." << endl;
            break;
        }
        vector<LoanSlip> slips = loanSlipService.getLoanSlipsByMember(memberId);
        cout << endl;
        cout << "Member_ID: " << memberId << endl;
        printLoanSlips(slips);
    }
    return 0;
}
int main(int argc, char* argv[]) {
    bool apiMode = argc > 1 && string(argv[1]) == "--api";
    JsonDatabase database("data/library.json");
    json data = database.load();
    if (data.empty()) {
        if (apiMode) {
            cout << json{ {"success", false}, {"error", "Khong load duoc library.json."} }.dump();
            return 1;
        }
        cout << "Khong load duoc library.json." << endl;
        return 1;
    }
    vector<Book> books = JsonMapper::booksFromJson(data);
    vector<Member> members = JsonMapper::membersFromJson(data);
    vector<Loan> loans = JsonMapper::loansFromJson(data);
    vector<Fine> fines = JsonMapper::finesFromJson(data);
    BookRepository bookRepository;
    MemberRepository memberRepository;
    LoanRepository loanRepository;
    FineRepository fineRepository;
    bookRepository.getAll() = books;
    memberRepository.getAll() = members;
    loanRepository.getAll() = loans;
    fineRepository.getAll() = fines;
    BookService bookService(bookRepository);
    LoanSlipService loanSlipService(loanRepository, memberRepository, bookRepository);
    LoanService loanService(loanRepository, bookRepository, fineRepository);
    if (apiMode) {
        return runApiMode(loanSlipService, memberRepository, bookService, bookRepository, loanRepository, loanService, fineRepository, database, data);
    }
    return runConsoleMode(loanSlipService, bookService);
}