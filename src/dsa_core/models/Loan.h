#ifndef LOAN_H
#define LOAN_H
#include <string>
using namespace std;
struct Loan{
    string loanId;
    string memberId;
    string bookId;
    string borrowDate;
    string dueDate;
    string returnDate;
    int renewalCount;
    string status;
};
#endif