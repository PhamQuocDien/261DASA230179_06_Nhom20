#include "ReservationRepository.h"
ReservationRepository::ReservationRepository() {}
std::vector<Reservation>& ReservationRepository::getAll() {
    return reservations;
}
const std::vector<Reservation>& ReservationRepository::getAll() const {
    return reservations;
}
Reservation* ReservationRepository::findById(const std::string& reservationId) {
    for (Reservation& reservation : reservations) {
        if (reservation.reservationId == reservationId) {
            return &reservation;
        }
    }
    return nullptr;
}
const Reservation* ReservationRepository::findById(const std::string& reservationId) const {
    for (const Reservation& reservation : reservations) {
        if (reservation.reservationId == reservationId) {
            return &reservation;
        }
    }
    return nullptr;
}
bool ReservationRepository::add(const Reservation& reservation) {
    if (findById(reservation.reservationId) != nullptr) {
        return false;
    }
    reservations.push_back(reservation);
    return true;
}
bool ReservationRepository::update(const Reservation& reservation) {
    Reservation* existingReservation = findById(reservation.reservationId);
    if (existingReservation == nullptr) {
        return false;
    }
    *existingReservation = reservation;
    return true;
}
bool ReservationRepository::removeById(const std::string& reservationId) {
    for (auto it = reservations.begin(); it != reservations.end(); ++it) {
        if (it->reservationId == reservationId) {
            reservations.erase(it);
            return true;
        }
    }
    return false;
}