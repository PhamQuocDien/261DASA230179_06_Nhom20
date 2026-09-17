#ifndef FINE_H
#define FINE_H
#include <string>
using namespace std;
struct Fine
{
    string fineId;
    string loanId;
    string memberId;
    double amount;
    string reason;

    string status;
};
#endif