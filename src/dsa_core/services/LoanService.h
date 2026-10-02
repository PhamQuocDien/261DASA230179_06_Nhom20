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
#include "../repositories/MemberRepository.h"
#include "../services/ReservationService.h"

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

struct BorrowResult {
    bool isSuccess;
    string message;
    Loan loan;
};

class LoanService {
private:
    LoanRepository& loanRepo;
    BookRepository& bookRepo;
    FineRepository& fineRepo;
    MemberRepository& memberRepo;
    ReservationService& reservationService;

    string generateLoanId();

public:
    LoanService(LoanRepository& lr, BookRepository& br, FineRepository& fr, MemberRepository& mr, ReservationService& rs)
        : loanRepo(lr), bookRepo(br), fineRepo(fr), memberRepo(mr), reservationService(rs) {
    }
    static string calculateDueDate(const string& borrowDateStr, int daysToAdd);
    ReturnReceipt returnBook(const string& loanId, const Date& returnDate, const string& quality);
    BorrowResult borrowBook(const string& memberId, const string& bookCode, const string& borrowDateStr);
};

#endif
