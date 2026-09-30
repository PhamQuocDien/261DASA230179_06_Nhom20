#ifndef RENEW_SERVICE_H
#define RENEW_SERVICE_H

#include <string>

#include "../models/Loan.h"
#include "../models/Date.h"
#include "../models/Book.h"
#include "../repositories/LoanRepository.h"
#include "../repositories/BookRepository.h"
#include "../repositories/ReservationRepository.h"
#include "../services/LoanService.h"

using namespace std;

struct RenewResult {
    bool isSuccess;
    string message;
    Loan loan;
};

class RenewService {
private:
    LoanRepository& loanRepo;
    BookRepository& bookRepo;
    ReservationRepository& reservationRepo;

    string calculateNewDueDate(const string& oldDueDate, int daysToAdd);

public:
    RenewService(
        LoanRepository& lr,
        BookRepository& br,
        ReservationRepository& rr
    );

    RenewResult renewBook(const string& loanId);
};

#endif
