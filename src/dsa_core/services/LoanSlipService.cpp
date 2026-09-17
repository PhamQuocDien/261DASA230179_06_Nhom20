#ifndef MERGESORT_H
#define MERGESORT_H
#include <algorithm>
#include <vector>
#include "../models/Book.h"
void sortBooksByYear(std::vector<Book>& books) {
    std::sort(books.begin(), books.end(), [](const Book& a, const Book& b) {
        return a.year < b.year;
        });
}
#endif
#include "LoanSlipService.h"
#include <algorithm>
using namespace std;
LoanSlipService::LoanSlipService(LoanRepository& loanRepository, MemberRepository& memberRepository, BookRepository& bookRepository)
    : loanRepository(loanRepository), memberRepository(memberRepository), bookRepository(bookRepository), memberLoanIndex(10) {
    buildMemberLoanIndex();
}
void LoanSlipService::buildMemberLoanIndex() {
    const vector<Loan>& loans = loanRepository.getAll();
    for (const Loan& loan : loans) {
        vector<string>* loanIds = memberLoanIndex.find(loan.memberId);
        if (loanIds == nullptr) {
            memberLoanIndex.insert(loan.memberId, vector<string>{ loan.loanId });
        }
        else {
            loanIds->push_back(loan.loanId);
        }
    }
}
const Member* LoanSlipService::findMemberById(const string& memberId) const {
    if (memberId.empty()) {
        return nullptr;
    }
    const vector<Member>& members = memberRepository.getAll();
    for (const Member& member : members) {
        if (member.memberId == memberId) {
            return &member;
        }
    }
    return nullptr;
}
const Book* LoanSlipService::findBookByCopyId(const string& bookId) const {
    if (bookId.empty()) {
        return nullptr;
    }
    const vector<Book>& books = bookRepository.getAll();
    for (const Book& book : books) {
        for (const BookCopy& copy : book.copies) {
            if (copy.bookId == bookId) {
                return &book;
            }
        }
    }
    return nullptr;
}
bool LoanSlipService::createLoanSlip(const Loan& loan, LoanSlip& result) const {
    const Member* member = findMemberById(loan.memberId);
    if (member == nullptr) {
        return false;
    }
    const Book* book = findBookByCopyId(loan.bookId);
    if (book == nullptr) {
        return false;
    }
    result.loanId = loan.loanId;
    result.memberId = loan.memberId;
    result.memberName = member->name;
    result.bookId = loan.bookId;
    result.bookCode = book->bookCode;
    result.bookTitle = book->title;
    result.borrowDate = loan.borrowDate;
    result.dueDate = loan.dueDate;
    result.returnDate = loan.returnDate;
    result.renewalCount = loan.renewalCount;
    result.status = loan.status;
    return true;
}
vector<LoanSlip> LoanSlipService::getLoanSlipsByMember(const string& memberId) const {
    vector<LoanSlip> result;
    if (memberId.empty()) {
        return result;
    }
    const Member* member = findMemberById(memberId);
    if (member == nullptr) {
        return result;
    }
    const vector<string>* loanIds = memberLoanIndex.find(memberId);
    if (loanIds == nullptr) {
        return result;
    }
    for (const string& loanId : *loanIds) {
        const Loan* loan = loanRepository.findById(loanId);
        if (loan == nullptr) {
            continue;
        }
        LoanSlip slip;
        if (createLoanSlip(*loan, slip)) {
            result.push_back(slip);
        }
    }
    sort(result.begin(), result.end(), [](const LoanSlip& a, const LoanSlip& b) {
        if (a.borrowDate != b.borrowDate) {
            return a.borrowDate < b.borrowDate;
        }
        return a.loanId < b.loanId;
        });
    return result;
}
bool LoanSlipService::getLoanSlipByLoanId(const string& loanId, LoanSlip& result) const {
    if (loanId.empty()) {
        return false;
    }
    const Loan* loan = loanRepository.findById(loanId);
    if (loan == nullptr) {
        return false;
    }
    return createLoanSlip(*loan, result);
}