#include "JsonMapper.h"

using namespace std;
using nlohmann::json;

namespace JsonMapper
{
    // =========================================================
    // BOOK
    // =========================================================

    json bookToJson(const Book& book)
    {
        json j;

        j["bookCode"] = book.BookCode;
        j["bookId"] = book.Book_ID;
        j["title"] = book.Title;
        j["author"] = book.Author;
        j["category"] = book.Category;
        j["year"] = book.Year;
        j["quantity"] = book.Quantity;
        j["interestCount"] = book.interestCount;

        return j;
    }

    Book bookFromJson(const json& j)
    {
        Book book;

        if (j.contains("bookCode"))
            book.BookCode = j["bookCode"];

        if (j.contains("bookId"))
            book.Book_ID = j["bookId"];

        if (j.contains("title"))
            book.Title = j["title"];

        if (j.contains("author"))
            book.Author = j["author"];

        if (j.contains("category"))
            book.Category = j["category"];

        if (j.contains("year"))
            book.Year = j["year"];

        if (j.contains("quantity"))
            book.Quantity = j["quantity"];

        if (j.contains("interestCount"))
            book.interestCount = j["interestCount"];

        return book;
    }


    // =========================================================
    // MEMBER
    // =========================================================

    json memberToJson(const Member& member)
    {
        json j;

        j["memberId"] = member.memberId;
        j["name"] = member.name;
        j["email"] = member.email;
        j["phone"] = member.phone;
        j["status"] = member.status;

        return j;
    }

    Member memberFromJson(const json& j)
    {
        Member member;

        if (j.contains("memberId"))
            member.memberId = j["memberId"];

        if (j.contains("name"))
            member.name = j["name"];

        if (j.contains("email"))
            member.email = j["email"];

        if (j.contains("phone"))
            member.phone = j["phone"];

        if (j.contains("status"))
            member.status = j["status"];

        return member;
    }


    // =========================================================
    // LOAN
    // =========================================================

    json loanToJson(const Loan& loan)
    {
        json j;

        j["loanId"] = loan.loanId;
        j["memberId"] = loan.memberId;
        j["bookId"] = loan.bookId;
        j["borrowDate"] = loan.borrowDate;
        j["dueDate"] = loan.dueDate;

        if (loan.returnDate.empty())
        {
            j["returnDate"] = nullptr;
        }
        else
        {
            j["returnDate"] = loan.returnDate;
        }

        j["renewalCount"] = loan.renewalCount;
        j["status"] = loan.status;

        return j;
    }

    Loan loanFromJson(const json& j)
    {
        Loan loan;

        if (j.contains("loanId"))
            loan.loanId = j["loanId"];

        if (j.contains("memberId"))
            loan.memberId = j["memberId"];

        if (j.contains("bookId"))
            loan.bookId = j["bookId"];

        if (j.contains("borrowDate"))
            loan.borrowDate = j["borrowDate"];

        if (j.contains("dueDate"))
            loan.dueDate = j["dueDate"];

        if (!j.contains("returnDate") || j["returnDate"].is_null())
        {
            loan.returnDate = "";
        }
        else
        {
            loan.returnDate = j["returnDate"];
        }

        if (j.contains("renewalCount"))
            loan.renewalCount = j["renewalCount"];

        if (j.contains("status"))
            loan.status = j["status"];

        return loan;
    }


    // =========================================================
    // RESERVATION
    // =========================================================

    json reservationToJson(const Reservation& reservation)
    {
        json j;

        j["reservationId"] = reservation.reservationId;
        j["memberId"] = reservation.memberId;
        j["bookCode"] = reservation.bookCode;
        j["reservationDate"] = reservation.reservationDate;
        j["status"] = reservation.status;

        return j;
    }

    Reservation reservationFromJson(const json& j)
    {
        Reservation reservation;

        if (j.contains("reservationId"))
            reservation.reservationId = j["reservationId"];

        if (j.contains("memberId"))
            reservation.memberId = j["memberId"];

        if (j.contains("bookCode"))
            reservation.bookCode = j["bookCode"];

        if (j.contains("reservationDate"))
            reservation.reservationDate = j["reservationDate"];

        if (j.contains("status"))
            reservation.status = j["status"];

        return reservation;
    }


    // =========================================================
    // FINE
    // =========================================================

    json fineToJson(const Fine& fine)
    {
        json j;

        j["fineId"] = fine.fineId;
        j["loanId"] = fine.loanId;
        j["memberId"] = fine.memberId;
        j["amount"] = fine.amount;
        j["reason"] = fine.reason;
        j["status"] = fine.status;

        return j;
    }

    Fine fineFromJson(const json& j)
    {
        Fine fine;

        if (j.contains("fineId"))
            fine.fineId = j["fineId"];

        if (j.contains("loanId"))
            fine.loanId = j["loanId"];

        if (j.contains("memberId"))
            fine.memberId = j["memberId"];

        if (j.contains("amount"))
            fine.amount = j["amount"];

        if (j.contains("reason"))
            fine.reason = j["reason"];

        if (j.contains("status"))
            fine.status = j["status"];

        return fine;
    }


    // =========================================================
    // LOAD BOOKS
    // =========================================================

    vector<Book> booksFromJson(const json& data)
    {
        vector<Book> books;

        if (!data.contains("books") || !data["books"].is_array())
        {
            return books;
        }

        for (const auto& item : data["books"])
        {
            books.push_back(bookFromJson(item));
        }

        return books;
    }


    // =========================================================
    // LOAD MEMBERS
    // =========================================================

    vector<Member> membersFromJson(const json& data)
    {
        vector<Member> members;

        if (!data.contains("members") || !data["members"].is_array())
        {
            return members;
        }

        for (const auto& item : data["members"])
        {
            members.push_back(memberFromJson(item));
        }

        return members;
    }


    // =========================================================
    // LOAD LOANS
    // =========================================================

    vector<Loan> loansFromJson(const json& data)
    {
        vector<Loan> loans;

        if (!data.contains("loans") || !data["loans"].is_array())
        {
            return loans;
        }

        for (const auto& item : data["loans"])
        {
            loans.push_back(loanFromJson(item));
        }

        return loans;
    }


    // =========================================================
    // LOAD RESERVATIONS
    // =========================================================

    vector<Reservation> reservationsFromJson(const json& data)
    {
        vector<Reservation> reservations;

        if (!data.contains("reservations") ||
            !data["reservations"].is_array())
        {
            return reservations;
        }

        for (const auto& item : data["reservations"])
        {
            reservations.push_back(reservationFromJson(item));
        }

        return reservations;
    }


    // =========================================================
    // LOAD FINES
    // =========================================================

    vector<Fine> finesFromJson(const json& data)
    {
        vector<Fine> fines;

        if (!data.contains("fines") ||
            !data["fines"].is_array())
        {
            return fines;
        }

        for (const auto& item : data["fines"])
        {
            fines.push_back(fineFromJson(item));
        }

        return fines;
    }


    // =========================================================
    // SAVE ENTIRE LIBRARY
    // =========================================================

    json libraryToJson(
        const vector<Book>& books,
        const vector<Member>& members,
        const vector<Loan>& loans,
        const vector<Reservation>& reservations,
        const vector<Fine>& fines
    )
    {
        json data;

        data["books"] = json::array();
        data["members"] = json::array();
        data["loans"] = json::array();
        data["reservations"] = json::array();
        data["fines"] = json::array();


        for (const Book& book : books)
        {
            data["books"].push_back(bookToJson(book));
        }


        for (const Member& member : members)
        {
            data["members"].push_back(memberToJson(member));
        }


        for (const Loan& loan : loans)
        {
            data["loans"].push_back(loanToJson(loan));
        }


        for (const Reservation& reservation : reservations)
        {
            data["reservations"].push_back(
                reservationToJson(reservation)
            );
        }


        for (const Fine& fine : fines)
        {
            data["fines"].push_back(fineToJson(fine));
        }

        return data;
    }
}
