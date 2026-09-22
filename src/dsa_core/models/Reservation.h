#ifndef RESERVATION_H
#define RESERVATION_H

#include <string>

using namespace std;

struct Reservation
{
    string reservationId;

    string memberId;

    // Reservation theo mã đầu sách
    string bookCode;

    string reservationDate;

    string status;
};

#endif
