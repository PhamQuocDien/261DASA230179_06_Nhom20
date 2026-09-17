#ifndef RESERVATIONREPOSITORY_H
#define RESERVATIONREPOSITORY_H
#include <string>
#include <vector>
#include "../models/Reservation.h"
class ReservationRepository {
private:
    std::vector<Reservation> reservations;
public:
    ReservationRepository();
    std::vector<Reservation>& getAll();
    const std::vector<Reservation>& getAll() const;
    Reservation* findById(const std::string& reservationId);
    const Reservation* findById(const std::string& reservationId) const;
    bool add(const Reservation& reservation);
    bool update(const Reservation& reservation);
    bool removeById(const std::string& reservationId);
};
#endif