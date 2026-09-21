#pragma once

#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

#include "MemberRepository.h"
#include "BookRepository.h"
#include "LoanRepository.h"
#include "ReservationRepository.h"
#include "Reservation.h"

class ReservationService {
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
    bool enqueue(const string& memberId, const string& bookId);
    bool cancel(const string& reservationId);
    Reservation* next(const string& bookId);
};