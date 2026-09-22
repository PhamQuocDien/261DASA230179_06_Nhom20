#include "ReservationService.h"
#include <cctype>

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
    // =================================================
    // KHÔI PHỤC RESERVATION TỪ REPOSITORY
    // SAU KHI C++ PROCESS KHỞI ĐỘNG LẠI
    // =================================================

    unsigned long long maxReservationNumber = 0;

    for (const Reservation& reservation :
         reservationRepository.getAll())
    {
        // -------------------------------------------------
        // Khôi phục Queue cho các Reservation đang WAITING
        // -------------------------------------------------

        if (reservation.status == "WAITING")
        {
            waitQueues[reservation.bookCode].push(
                reservation.reservationId
            );
        }

        // -------------------------------------------------
        // Tìm Reservation_ID lớn nhất
        // Ví dụ: R001 -> 1
        //        R002 -> 2
        //        R015 -> 15
        // -------------------------------------------------

        const string& reservationId =
            reservation.reservationId;

        if (reservationId.size() < 2)
            continue;

        if (reservationId[0] != 'R' &&
            reservationId[0] != 'r')
        {
            continue;
        }

        bool numeric = true;
        unsigned long long number = 0;

        for (size_t i = 1;
             i < reservationId.size();
             ++i)
        {
            if (!isdigit(
                    static_cast<unsigned char>(
                        reservationId[i]
                    )
                ))
            {
                numeric = false;
                break;
            }

            number =
                number * 10 +
                static_cast<unsigned long long>(
                    reservationId[i] - '0'
                );
        }

        if (numeric &&
            number > maxReservationNumber)
        {
            maxReservationNumber = number;
        }
    }

    // Reservation tiếp theo phải lớn hơn ID lớn nhất
    nextReservationId =
        static_cast<int>(maxReservationNumber + 1);
}


bool ReservationService::enqueue(
    const string& memberId,
    const string& bookCode
)
{
    // =================================================
    // 1. Kiểm tra Member có tồn tại không
    // =================================================

    Member* member =
        memberRepository.findById(memberId);

    if (member == nullptr)
        return false;


    // =================================================
    // 2. Kiểm tra đầu sách có tồn tại không
    // =================================================

    Book* book =
        bookRepository.findByCode(bookCode);

    if (book == nullptr)
        return false;


    // =================================================
    // 3. Chỉ cho đăng ký chờ khi tất cả BookCopy
    //    của đầu sách đều không còn available
    // =================================================

    bool hasAvailableCopy = false;

    for (const BookCopy& copy :
         book->copies)
    {
        if (copy.status == "available")
        {
            hasAvailableCopy = true;
            break;
        }
    }

    // Vẫn còn bản sách có thể mượn
    // => không cần đăng ký chờ
    if (hasAvailableCopy)
        return false;


    // =================================================
    // 4. Kiểm tra Member có đang mượn đầu sách này không
    //
    // Reservation dùng bookCode
    // Loan dùng bookId của BookCopy
    //
    // Vì vậy phải kiểm tra từng BookCopy
    // =================================================

    bool memberIsBorrowingBook = false;

    for (const BookCopy& copy :
         book->copies)
    {
        Loan* loan =
            loanRepository.findByMemberAndBook(
                memberId,
                copy.bookId
            );

        if (loan != nullptr &&
            loan->status == "BORROWING")
        {
            memberIsBorrowingBook = true;
            break;
        }
    }

    if (memberIsBorrowingBook)
        return false;


    // =================================================
    // 5. Kiểm tra Member đã có Reservation WAITING
    //    cho đầu sách này chưa
    // =================================================

    for (const auto& reservation :
         reservationRepository.getAll())
    {
        if (reservation.memberId == memberId &&
            reservation.bookCode == bookCode &&
            reservation.status == "WAITING")
        {
            return false;
        }
    }


    // =================================================
    // 6. Tạo Reservation mới
    // =================================================

    string reservationId =
        "R" + to_string(nextReservationId++);

    Reservation r;

    r.reservationId =
        reservationId;

    r.memberId =
        memberId;

    r.bookCode =
        bookCode;

    r.reservationDate =
        getCurrentTime();

    r.status =
        "WAITING";


    // =================================================
    // 7. Lưu Reservation vào Repository
    // =================================================

    reservationRepository.add(r);


    // =================================================
    // 8. Đưa Reservation vào Queue
    //    theo thứ tự đăng ký FIFO
    // =================================================

    waitQueues[bookCode].push(
        reservationId
    );

    return true;
}


bool ReservationService::cancel(
    const string& reservationId
)
{
    // =================================================
    // 1. Tìm Reservation
    // =================================================

    Reservation* reservation =
        reservationRepository.findById(
            reservationId
        );

    if (reservation == nullptr)
        return false;


    // =================================================
    // 2. Chỉ Reservation WAITING mới được hủy
    // =================================================

    if (reservation->status != "WAITING")
        return false;


    // =================================================
    // 3. Chuyển sang CANCELLED
    //
    // Reservation vẫn còn trong Queue.
    // Khi next() gặp nó sẽ bỏ qua.
    // =================================================

    reservation->status =
        "CANCELLED";

    return true;
}


Reservation* ReservationService::next(
    const string& bookCode
)
{
    // =================================================
    // 1. Tìm Queue của đầu sách
    // =================================================

    auto it =
        waitQueues.find(bookCode);

    if (it == waitQueues.end())
        return nullptr;

    if (it->second.empty())
        return nullptr;


    // =================================================
    // 2. Duyệt Queue theo FIFO
    // =================================================

    while (!it->second.empty())
    {
        // Reservation đứng đầu Queue
        string reservationId =
            it->second.front();


        // =================================================
        // 3. Tìm Reservation trong Repository
        // =================================================

        Reservation* reservation =
            reservationRepository.findById(
                reservationId
            );


        // =================================================
        // 4. Reservation không tồn tại
        //    hoặc không còn WAITING
        //    => bỏ qua
        // =================================================

        if (reservation == nullptr ||
            reservation->status != "WAITING")
        {
            it->second.pop();
            continue;
        }


        // =================================================
        // 5. Reservation hợp lệ đầu tiên
        //    => phục vụ
        // =================================================

        it->second.pop();

        reservation->status =
            "SERVED";

        return reservation;
    }

    return nullptr;
}
