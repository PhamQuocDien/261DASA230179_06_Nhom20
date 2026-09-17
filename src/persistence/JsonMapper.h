#ifndef JSONMAPPER_H
#define JSONMAPPER_H
#include <vector>
#include <nlohmann/json.hpp>
#include "../dsa_core/models/Book.h"
#include "../dsa_core/models/Member.h"
#include "../dsa_core/models/Loan.h"
#include "../dsa_core/models/Reservation.h"
#include "../dsa_core/models/Fine.h"
namespace JsonMapper {
    nlohmann::json bookToJson(const Book& book);
    Book bookFromJson(const nlohmann::json& j);
    nlohmann::json memberToJson(const Member& member);
    Member memberFromJson(const nlohmann::json& j);
    nlohmann::json loanToJson(const Loan& loan);
    Loan loanFromJson(const nlohmann::json& j);
    nlohmann::json reservationToJson(const Reservation& reservation);
    Reservation reservationFromJson(const nlohmann::json& j);
    nlohmann::json fineToJson(const Fine& fine);
    Fine fineFromJson(const nlohmann::json& j);
    std::vector<Book> booksFromJson(const nlohmann::json& data);
    std::vector<Member> membersFromJson(const nlohmann::json& data);
    std::vector<Loan> loansFromJson(const nlohmann::json& data);
    std::vector<Reservation> reservationsFromJson(const nlohmann::json& data);
    std::vector<Fine> finesFromJson(const nlohmann::json& data);
    nlohmann::json libraryToJson(const std::vector<Book>& books, const std::vector<Member>& members, const std::vector<Loan>& loans, const std::vector<Reservation>& reservations, const std::vector<Fine>& fines);
}
#endif