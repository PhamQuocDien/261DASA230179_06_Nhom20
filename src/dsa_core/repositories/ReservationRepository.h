#ifndef RESERVATIONREPOSITORY_H
#define RESERVATIONREPOSITORY_H

#include "../models/Reservation.h"

#include <string>
#include <vector>

using namespace std;

class ReservationRepository
{
private:
    vector<Reservation> reservations;

public:
    void add(
        const Reservation& reservation
    );

    Reservation* findById(
        const string& reservationId
    );

    bool update(
        const Reservation& reservation
    );

    bool remove(
        const string& reservationId
    );

    const vector<Reservation>& getAll() const;

    Reservation* findByMemberAndBook(
        const string& memberId,
        const string& bookCode
    );
};

#endif
