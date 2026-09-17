#ifndef RESERVATION_H
#define RESERVATION_H
#include <string>
using namespace std;
struct Reservation
{
    string reservationId;
    string memberId;
    string bookCode;
    string reservationDate;
    string status;
};

#endif