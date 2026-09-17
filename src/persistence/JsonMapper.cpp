#include "JsonMapper.h"
using namespace std;
using nlohmann::json;
namespace JsonMapper {
    json bookToJson(const Book& book) {
        json j;
        j["bookCode"] = book.bookCode;
        j["title"] = book.title;
        j["author"] = book.author;
        j["category"] = book.category;
        j["year"] = book.year;
        j["copies"] = json::array();
        for (const BookCopy& copy : book.copies) {
            json copyJson;
            copyJson["bookId"] = copy.bookId;
            copyJson["status"] = copy.status;
            j["copies"].push_back(copyJson);
        }
        return j;
    }
    Book bookFromJson(const json& j) {
        Book book;
        book.bookCode = j["bookCode"];
        book.title = j["title"];
        book.author = j["author"];
        book.category = j["category"];
        book.year = j["year"];
        if (j.contains("copies") && j["copies"].is_array()) {
            for (const auto& copyJson : j["copies"]) {
                BookCopy copy;
                copy.bookId = copyJson["bookId"];
                copy.status = copyJson["status"];
                book.copies.push_back(copy);
            }
        }
        return book;
    }
    json memberToJson(const Member& member) {
        json j;
        j["memberId"] = member.memberId;
        j["name"] = member.name;
        j["email"] = member.email;
        j["phone"] = member.phone;
        j["status"] = member.status;
        return j;
    }
    Member memberFromJson(const json& j) {
        Member member;
        member.memberId = j["memberId"];
        member.name = j["name"];
        member.email = j["email"];
        member.phone = j["phone"];
        member.status = j["status"];
        return member;
    }
    json loanToJson(const Loan& loan) {
        json j;
        j["loanId"] = loan.loanId;
        j["memberId"] = loan.memberId;
        j["bookId"] = loan.bookId;
        j["borrowDate"] = loan.borrowDate;
        j["dueDate"] = loan.dueDate;
        if (loan.returnDate.empty()) {
            j["returnDate"] = nullptr;
        }
        else {
            j["returnDate"] = loan.returnDate;
        }
        j["renewalCount"] = loan.renewalCount;
        j["status"] = loan.status;
        return j;
    }
    Loan loanFromJson(const json& j) {
        Loan loan;
        loan.loanId = j["loanId"];
        loan.memberId = j["memberId"];
        loan.bookId = j["bookId"];
        loan.borrowDate = j["borrowDate"];
        loan.dueDate = j["dueDate"];
        if (!j.contains("returnDate") || j["returnDate"].is_null()) {
            loan.returnDate = "";
        }
        else {
            loan.returnDate = j["returnDate"];
        }
        loan.renewalCount = j["renewalCount"];
        loan.status = j["status"];
        return loan;
    }
    json reservationToJson(const Reservation& reservation) {
        json j;
        j["reservationId"] = reservation.reservationId;
        j["memberId"] = reservation.memberId;
        j["bookCode"] = reservation.bookCode;
        j["reservationDate"] = reservation.reservationDate;
        j["status"] = reservation.status;
        return j;
    }
    Reservation reservationFromJson(const json& j) {
        Reservation reservation;
        reservation.reservationId = j["reservationId"];
        reservation.memberId = j["memberId"];
        reservation.bookCode = j["bookCode"];
        reservation.reservationDate = j["reservationDate"];
        reservation.status = j["status"];
        return reservation;
    }
    json fineToJson(const Fine& fine) {
        json j;
        j["fineId"] = fine.fineId;
        j["loanId"] = fine.loanId;
        j["memberId"] = fine.memberId;
        j["amount"] = fine.amount;
        j["reason"] = fine.reason;
        j["status"] = fine.status;
        return j;
    }
    Fine fineFromJson(const json& j) {
        Fine fine;
        fine.fineId = j["fineId"];
        fine.loanId = j["loanId"];
        fine.memberId = j["memberId"];
        fine.amount = j["amount"];
        fine.reason = j["reason"];
        fine.status = j["status"];
        return fine;
    }
    vector<Book> booksFromJson(const json& data) {
        vector<Book> books;
        if (!data.contains("books") || !data["books"].is_array()) {
            return books;
        }
        for (const auto& item : data["books"]) {
            books.push_back(bookFromJson(item));
        }
        return books;
    }
    vector<Member> membersFromJson(const json& data) {
        vector<Member> members;
        if (!data.contains("members") || !data["members"].is_array()) {
            return members;
        }
        for (const auto& item : data["members"]) {
            members.push_back(memberFromJson(item));
        }
        return members;
    }
    vector<Loan> loansFromJson(const json& data) {
        vector<Loan> loans;
        if (!data.contains("loans") || !data["loans"].is_array()) {
            return loans;
        }
        for (const auto& item : data["loans"]) {
            loans.push_back(loanFromJson(item));
        }
        return loans;
    }
    vector<Reservation> reservationsFromJson(const json& data) {
        vector<Reservation> reservations;
        if (!data.contains("reservations") || !data["reservations"].is_array()) {
            return reservations;
        }
        for (const auto& item : data["reservations"]) {
            reservations.push_back(reservationFromJson(item));
        }
        return reservations;
    }
    vector<Fine> finesFromJson(const json& data) {
        vector<Fine> fines;
        if (!data.contains("fines") || !data["fines"].is_array()) {
            return fines;
        }
        for (const auto& item : data["fines"]) {
            fines.push_back(fineFromJson(item));
        }
        return fines;
    }
    json libraryToJson(const vector<Book>& books, const vector<Member>& members, const vector<Loan>& loans, const vector<Reservation>& reservations, const vector<Fine>& fines) {
        json data;
        data["books"] = json::array();
        data["members"] = json::array();
        data["loans"] = json::array();
        data["reservations"] = json::array();
        data["fines"] = json::array();
        for (const Book& book : books) {
            data["books"].push_back(bookToJson(book));
        }
        for (const Member& member : members) {
            data["members"].push_back(memberToJson(member));
        }
        for (const Loan& loan : loans) {
            data["loans"].push_back(loanToJson(loan));
        }
        for (const Reservation& reservation : reservations) {
            data["reservations"].push_back(reservationToJson(reservation));
        }
        for (const Fine& fine : fines) {
            data["fines"].push_back(fineToJson(fine));
        }
        return data;
    }
}