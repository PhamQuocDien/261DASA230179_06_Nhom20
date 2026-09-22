#pragma once

#include "Reservation.h"
#include <vector>
using namespace std;

class ReservationRepository {
private:
    vector<Reservation> reservations;

public:
    void add(const Reservation& reservation);
    Reservation* findById(const string& reservationId);
    bool update(const Reservation& reservation);
    bool remove(const string& reservationId);
    vector<Reservation>& getAll();

    Reservation* findByMemberAndBook( const string& memberId,const string& bookCode
    );
};
