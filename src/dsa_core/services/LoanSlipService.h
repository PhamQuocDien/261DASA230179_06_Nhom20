#ifndef LOANSLIPSERVICE_H
#define LOANSLIPSERVICE_H

#include <string>
#include <vector>

#include "../models/Book.h"
#include "../models/Member.h"
#include "../models/Loan.h"
#include "../repositories/BookRepository.h"
#include "../repositories/MemberRepository.h"
#include "../repositories/LoanRepository.h"
#include "../structures/HashTable.h"

struct LoanSlip {
    std::string loanId;
    std::string memberId;
    std::string memberName;
    std::string bookId;
    std::string bookCode;
    std::string bookTitle;
    std::string borrowDate;
    std::string dueDate;
    std::string returnDate;
    int renewalCount = 0;
    std::string status;
};

class LoanSlipService {
private:
    LoanRepository& loanRepository;
    MemberRepository& memberRepository;
    BookRepository& bookRepository;
    HashTable<std::string, std::vector<std::string>> memberLoanIndex;

private:
    const Member* findMemberById(const std::string& memberId) const;
    const Book* findBookByCopyId(const std::string& bookId) const;
    bool createLoanSlip(const Loan& loan, LoanSlip& result) const;
    void buildMemberLoanIndex();

public:
    LoanSlipService(
        LoanRepository& loanRepository,
        MemberRepository& memberRepository,
        BookRepository& bookRepository
    );

    std::vector<LoanSlip> getLoanSlipsByMember(
        const std::string& memberId
    ) const;

    bool getLoanSlipByLoanId(
        const std::string& loanId,
        LoanSlip& result
    ) const;
};

#endif