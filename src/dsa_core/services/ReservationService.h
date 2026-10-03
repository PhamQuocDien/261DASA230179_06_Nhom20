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

struct CancelReservationResult
{
    bool isSuccess;
    string message;
    string reservationId;
};

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
    ReservationService(MemberRepository& memberRepository, BookRepository& bookRepository, LoanRepository& loanRepository, ReservationRepository& reservationRepository);

    bool enqueue(const string& memberId, const string& bookCode);

    bool cancel(const string& reservationId);

    // Huy Reservation co xac thuc mat khau.
    // Chuoi xac thuc lay tu Reservation trong du lieu,
    // khong tin Member_ID do client gui len.
    CancelReservationResult cancelWithPassword(
        const string& reservationId,
        const string& password
    );

    Reservation* getNextEligible(const string& bookCode);

    bool serve(const string& reservationId);
};

#endif
