#include "ReservationService.h"

#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;


// =========================================================
// LẤY NGÀY HIỆN TẠI: YYYY-MM-DD
// =========================================================

static string getCurrentTime()
{
    time_t now = time(nullptr);

    tm localTime{};

#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif

    ostringstream stream;

    stream << put_time(
        &localTime,
        "%Y-%m-%d"
    );

    return stream.str();
}


// =========================================================
// CONSTRUCTOR
// =========================================================

ReservationService::ReservationService(
    MemberRepository& memberRepository,
    BookRepository& bookRepository,
    LoanRepository& loanRepository,
    ReservationRepository& reservationRepository
)
    : memberRepository(memberRepository),
      bookRepository(bookRepository),
      loanRepository(loanRepository),
      reservationRepository(reservationRepository)
{
    // Khôi phục Queue từ các Reservation WAITING
    for (const Reservation& reservation :
         reservationRepository.getAll())
    {
        if (reservation.status == "WAITING")
        {
            waitQueues[reservation.bookCode].push(
                reservation.reservationId
            );
        }
    }

    // Tìm Reservation_ID lớn nhất
    int maxId = 0;

    for (const Reservation& reservation :
         reservationRepository.getAll())
    {
        const string& id = reservation.reservationId;

        if (id.size() <= 1)
            continue;

        if (id[0] != 'R')
            continue;

        bool numeric = true;
        int value = 0;

        for (size_t i = 1; i < id.size(); ++i)
        {
            if (id[i] < '0' || id[i] > '9')
            {
                numeric = false;
                break;
            }

            value = value * 10 +
                    (id[i] - '0');
        }

        if (numeric && value > maxId)
        {
            maxId = value;
        }
    }

    nextReservationId = maxId + 1;
}


// =========================================================
// ENQUEUE
// =========================================================

bool ReservationService::enqueue(
    const string& memberId,
    const string& bookCode
)
{
    // 1. Kiểm tra Member
    Member* member =
        memberRepository.findById(memberId);

    if (member == nullptr)
        return false;


    // 2. Kiểm tra Book
    Book* book =
        bookRepository.findByCode(bookCode);

    if (book == nullptr)
        return false;


    // 3. Chỉ cho đăng ký chờ khi
    //    KHÔNG còn bản sách available

    bool hasAvailableCopy = false;

    for (const BookCopy& copy : book->copies)
    {
        if (copy.status == "available")
        {
            hasAvailableCopy = true;
            break;
        }
    }

    // Vẫn còn sách để mượn
    // -> không cần đăng ký chờ
    if (hasAvailableCopy)
        return false;


    // 4. Kiểm tra Member có đang mượn
    //    một bản của đầu sách này không

    for (const BookCopy& copy : book->copies)
    {
        for (const Loan& loan :
             loanRepository.getAll())
        {
            if (loan.memberId == memberId &&
                loan.bookId == copy.bookId &&
                loan.status == "borrowing")
            {
                return false;
            }
        }
    }


    // 5. Member đã có Reservation WAITING
    //    cho Book này chưa

    for (const Reservation& reservation :
         reservationRepository.getAll())
    {
        if (reservation.memberId == memberId &&
            reservation.bookCode == bookCode &&
            reservation.status == "WAITING")
        {
            return false;
        }
    }


    // 6. Tạo Reservation mới

    string reservationId =
        "R" + to_string(nextReservationId++);

    Reservation reservation;

    reservation.reservationId =
        reservationId;

    reservation.memberId =
        memberId;

    reservation.bookCode =
        bookCode;

    reservation.reservationDate =
        getCurrentTime();

    reservation.status =
        "WAITING";


    // 7. Lưu Repository

    reservationRepository.add(
        reservation
    );


    // 8. Đưa vào Queue FIFO

    waitQueues[bookCode].push(
        reservationId
    );

    return true;
}


// =========================================================
// CANCEL
// =========================================================

bool ReservationService::cancel(
    const string& reservationId
)
{
    Reservation* reservation =
        reservationRepository.findById(
            reservationId
        );

    if (reservation == nullptr)
        return false;


    // Chỉ WAITING mới được hủy

    if (reservation->status != "WAITING")
        return false;


    // Không cần xóa khỏi queue.
    // next() sẽ bỏ qua CANCELLED.

    reservation->status =
        "CANCELLED";

    return true;
}


// =========================================================
// NEXT RESERVATION
// =========================================================

Reservation* ReservationService::next(
    const string& bookCode
)
{
    auto it =
        waitQueues.find(bookCode);

    if (it == waitQueues.end())
        return nullptr;


    if (it->second.empty())
        return nullptr;


    // FIFO

    while (!it->second.empty())
    {
        string reservationId =
            it->second.front();

        Reservation* reservation =
            reservationRepository.findById(
                reservationId
            );


        // Không tồn tại hoặc không còn WAITING
        // -> bỏ khỏi Queue

        if (reservation == nullptr ||
            reservation->status != "WAITING")
        {
            it->second.pop();
            continue;
        }


        // Reservation hợp lệ đầu tiên

        it->second.pop();

        reservation->status =
            "SERVED";

        return reservation;
    }

    return nullptr;
}
