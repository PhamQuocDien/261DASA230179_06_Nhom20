#include "LoanRepository.h"
LoanRepository::LoanRepository() {}
std::vector<Loan>& LoanRepository::getAll() {
    return loans;
}
const std::vector<Loan>& LoanRepository::getAll() const {
    return loans;
}
Loan* LoanRepository::findById(const std::string& loanId) {
    for (Loan& loan : loans) {
        if (loan.loanId == loanId) {
            return &loan;
        }
    }
    return nullptr;
}
const Loan* LoanRepository::findById(const std::string& loanId) const {
    for (const Loan& loan : loans) {
        if (loan.loanId == loanId) {
            return &loan;
        }
    }
    return nullptr;
}

Loan* LoanRepository::findByMemberAndBook(const std::string& memberId, const std::string& bookId) {
    for (Loan& loan : loans) {
        if (loan.memberId == memberId && loan.bookId == bookId) {
            return &loan;
        }
    }
    return nullptr;
}

const Loan* LoanRepository::findByMemberAndBook(const std::string& memberId, const std::string& bookId) const {
    for (const Loan& loan : loans) {
        if (loan.memberId == memberId && loan.bookId == bookId) {
            return &loan;
        }
    }
    return nullptr;
}

bool LoanRepository::add(const Loan& loan) {
    if (findById(loan.loanId) != nullptr) {
        return false;
    }
    loans.push_back(loan);
    return true;
}
bool LoanRepository::update(const Loan& loan) {
    Loan* existingLoan = findById(loan.loanId);
    if (existingLoan == nullptr) {
        return false;
    }
    *existingLoan = loan;
    return true;
}
bool LoanRepository::removeById(const std::string& loanId) {
    for (auto it = loans.begin(); it != loans.end(); ++it) {
        if (it->loanId == loanId) {
            loans.erase(it);
            return true;
        }
    }
    return false;
}
