#ifndef RESERVATIONSERVICE_H
#define RESERVATIONSERVICE_H

#include <string>
#include <queue>
#include <unordered_map>

#include "../repositories/MemberRepository.h"
#include "../repositories/BookRepository.h"
#include "../repositories/LoanRepository.h"
#include "../repositories/ReservationRepository.h"

#include "../models/Reservation.h"

using namespace std;

class ReservationService
{
private:
    MemberRepository& memberRepository;
    BookRepository& bookRepository;
    LoanRepository& loanRepository;
    ReservationRepository& reservationRepository;

    int nextReservationId = 1;

    unordered_map<string, queue<string>> waitQueues;

public:
    ReservationService(
        MemberRepository& memberRepository,
        BookRepository& bookRepository,
        LoanRepository& loanRepository,
        ReservationRepository& reservationRepository
    );

    bool enqueue(
        const string& memberId,
        const string& bookCode
    );

    bool cancel(
        const string& reservationId
    );

    Reservation* next(
        const string& bookCode
    );
};

#endif
