#ifndef LOAN_SERVICE_H
#define LOAN_SERVICE_H

#include <string>
#include "../models/Date.h"
#include "../models/Loan.h"
#include "../models/Book.h"
#include "../models/Fine.h"
#include "../repositories/LoanRepository.h"
#include "../repositories/BookRepository.h"
#include "../repositories/FineRepository.h"

using namespace std;

struct ReturnReceipt {
    bool isSuccess;
    string message;
    string loanId;
    int lateDays;
    double lateFee;
    double damageFee;
    double totalFee;
};

class LoanService {
private:
    LoanRepository& loanRepo;
    BookRepository& bookRepo;
    FineRepository& fineRepo;

public:
    LoanService(LoanRepository& lr, BookRepository& br, FineRepository& fr)
        : loanRepo(lr), bookRepo(br), fineRepo(fr) {}

    ReturnReceipt returnBook(const string& loanId, const Date& returnDate, const string& quality);
};

#endif
