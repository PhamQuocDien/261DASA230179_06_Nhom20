#include "ReservationRepository.h"

void ReservationRepository::add(const Reservation& reservation)
{
    reservations.push_back(reservation);
}
Reservation* ReservationRepository::findById(const string& reservationId) 
{
    for(auto& reservation:reservations) 
    {
        if (reservation.reservationId == reservationId) 
        {
            return &reservation;
        }

    }
    return nullptr;

}
bool ReservationRepository::update(const Reservation& reservation) {
    Reservation *existing = findById( reservation.reservationId);//Reservation đang nằm trong vector
    //biến reservation đã được định nghĩa cấu trúc cảu Reservation )
    if (existing == nullptr) {
        return false;

    }
    *existing = reservation;
    return true;// cập nhật thành công
   
}
bool ReservationRepository::remove(const string& reservationId)
{
    for (auto it = reservations.begin();it != reservations.end();) {
        if (it->reservationId == reservationId) {
            it = reservations.erase(it);
            return true;

        }
        else
            ++it;

    }
    return false;
}
vector<Reservation>& ReservationRepository::getAll()
{
    return reservations;
}
Reservation* ReservationRepository::findByMemberAndBook(
    const string& memberId,
    const string& bookCode
)
{
    for (auto& reservation : reservations)
    {
        if (reservation.memberId == memberId &&
            reservation.bookCode == bookCode &&
            reservation.status == "WAITING")
        {
            return &reservation;
        }
    }

    return nullptr;
}
