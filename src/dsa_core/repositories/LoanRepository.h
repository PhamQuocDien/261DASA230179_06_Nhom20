#ifndef LOANREPOSITORY_H
#define LOANREPOSITORY_H
#include <string>
#include <vector>
#include "../models/Loan.h"
class LoanRepository {
private:
    std::vector<Loan> loans;
public:
    LoanRepository();
    std::vector<Loan>& getAll();
    const std::vector<Loan>& getAll() const;
    Loan* findById(const std::string& loanId);
    const Loan* findById(const std::string& loanId) const;

    bool add(const Loan& loan);
    bool update(const Loan& loan);
    bool removeById(const std::string& loanId);
};
#endif
