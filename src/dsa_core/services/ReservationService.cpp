#include "ReservationService.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

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
    ReservationRepository& reservationRepository,
    FineRepository& fineRepository
)
    : memberRepository(memberRepository),
      bookRepository(bookRepository),
      loanRepository(loanRepository),
      reservationRepository(reservationRepository),
      fineRepository(fineRepository)
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
    Book* book = bookRepository.findByCode(bookCode);

    if (book == nullptr) 
        return false;
    // 3. Chỉ cho đăng ký chờ khi
    //    KHÔNG còn bản sách available

    bool hasAvailableCopy = false;

    for (const BookCopy& copy : book->copies)
    {
        if ((copy.status == "available" ||
            copy.status == "AVAILABLE"))
        {
            hasAvailableCopy = true;
            break;
        }
    }

    // Vẫn còn sách để mượn
    // -> không cần đăng ký chờ
    if (hasAvailableCopy) {
        return false;
    }

    // 4. Kiểm tra Member có đang mượn
    //    một bản của đầu sách này không

    for (const BookCopy& copy : book->copies)
    {
        for (const Loan& loan :
             loanRepository.getAll())
        {
            if (loan.memberId == memberId &&
                loan.bookId == copy.bookId &&
                (loan.status == "borrowing" ||
                loan.status == "BORROWING"))
            {
                
                return false;
            }
        }
    }


    // 5. Member đã có Reservation WAITING
    //    cho Book này chưa
    if (reservationRepository.findByMemberAndBook(memberId, bookCode) != nullptr)
    {
    return false;
    }

    


    // 6. Tạo Reservation mới

    string reservationId =
        "R" + to_string(nextReservationId++);

    Reservation reservation;

    reservation.reservationId =reservationId;

    reservation.memberId = memberId;

    reservation.bookCode =bookCode;

    reservation.reservationDate = getCurrentTime();

    reservation.status = "WAITING";


    // 7. Lưu Repository

    reservationRepository.add( reservation );


    // 8. Đưa vào Queue FIFO

    waitQueues[bookCode].push(reservationId);

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

    reservation->status = "CANCELLED";

    return true;
}


// =========================================================
// CANCEL RESERVATION + XAC THUC MAT KHAU
// =========================================================

CancelReservationResult ReservationService::cancelWithPassword(
    const string& reservationId,
    const string& password
)
{
    CancelReservationResult result{};
    result.isSuccess = false;
    result.reservationId = reservationId;

    // Reservation khong ton tai
    Reservation* reservation =
        reservationRepository.findById(
            reservationId
        );

    if (reservation == nullptr)
    {
        result.message =
            "Khong tim thay yeu cau cho nay.";

        return result;
    }

    // Khong cho huy lai reservation da duoc xu ly
    if (reservation->status != "WAITING")
    {
        result.message =
            "Yeu cau cho nay khong con o trang thai cho.";

        return result;
    }

    // Chuoi xac thuc luon lay tu du lieu Reservation
    const Member* owner =
        memberRepository.findById(
            reservation->memberId
        );

    if (owner == nullptr)
    {
        result.message =
            "Khong tim thay thanh vien so huu yeu cau cho.";

        return result;
    }

    // Mat khau rong -> khong cho thao tac
    if (password.empty())
    {
        result.message =
            "Vui long nhap mat khau thanh vien.";

        return result;
    }

    if (owner->password != password)
    {
        result.message =
            "Mat khau khong dung.";

        return result;
    }

    // Xac thuc thanh cong -> chay logic huy hien co
    if (!cancel(reservationId))
    {
        result.message =
            "Khong the huy yeu cau cho.";

        return result;
    }

    result.isSuccess = true;
    result.message =
        "Da huy yeu cau cho thanh cong.";

    return result;
}

//KIEM TRA THANH VIEN CO CON FINE CHUA THANH TOAN
//Fine status trong du lieu chi co UNPAID va PAID.
//Chua co trang thai moi, khong them trang thai moi.
bool ReservationService::hasUnpaidFine(const string& memberId) {
    for (const Fine& fine : fineRepository.getAll()) {
        if (fine.memberId != memberId)
            continue;

        string status = fine.status;
        for (char& c : status) {
            c = static_cast<char>(
                toupper(static_cast<unsigned char>(c))
            );
        }

        if (status != "PAID")
            return true;
    }
    return false;
}

//GET NEXT ELIGIBL RESERVATION
Reservation* ReservationService::getNextEligible(const string& bookCode) {
    auto it = waitQueues.find(bookCode);
    if (it == waitQueues.end())
        return nullptr;
    while (!it->second.empty()) {
        string reservationId = it->second.front();
        Reservation* reservation = reservationRepository.findById(reservationId);

        //reservation không tồn tại
        if (reservation == nullptr) {
            it->second.pop();
            continue;
        }
        //đã canclled hoặc không còn waitting
        if (reservation->status != "WAITING") {
            it->second.pop();
            continue;
        }
        //kểm tra member
        Member* member = memberRepository.findById(reservation->memberId);
        if (member == nullptr || member->status != "ACTIVE") {
            it->second.pop();
            continue;
        }
        //kiểm tra book
        Book* book = bookRepository.findByCode(reservation->bookCode);
        if (book == nullptr) {
            it->second.pop();
            continue;

        }
        //kiểm tra member có đang mượn một bản của đầu sách này không
        bool alreadyBorrowing = false;
        for (const BookCopy& copy : book->copies) {
            for (const Loan& loan : loanRepository.getAll()) {
                if (loan.memberId == reservation->memberId && loan.bookId == copy.bookId
                    && (loan.status == "borrowing" || loan.status == "BORROWING")) {
                    alreadyBorrowing = true;
                    break;
                }


            }
            if (alreadyBorrowing)
                break;
        }
        //Người này không còn đủ điều kiện 
        if (alreadyBorrowing) {
            it->second.pop();
            continue;
        }
        //kiểm tra giới hạn số sách đang mượn
        int activeLoans = 0;
        for (const Loan& loan : loanRepository.getAll()) {
            if (loan.memberId == reservation->memberId
                && (loan.status == "borrowing" || loan.status == "BORROWING")) {
                activeLoans++;
            }
        }
        //đã đạt giới hạn mượn
        if (activeLoans >= 10) {
            it->second.pop();
            continue;
        }
        //còn tiền phạt chưa thanh toán -> chưa đủ điều kiện mượn
        if (hasUnpaidFine(reservation->memberId)) {
            it->second.pop();
            continue;
        }

        // Người đầu tiên còn đủ điều kiện
        // CHƯA pop
        // CHƯA chuyển SERVED
        return reservation;
    }

    return nullptr;
}


         

   
// =========================================================
// SERVE RESERVATION
// =========================================================
bool ReservationService::serve(const string& reservationId) {
    Reservation* reservation = reservationRepository.findById(reservationId);
    if (reservation == nullptr)
        return false;

    if (reservation->status != "WAITING")
        return false;

    auto it = waitQueues.find(reservation->bookCode);
    if (it == waitQueues.end())
        return false;
    if (it->second.empty())
        return false;
    // Chỉ được SERVE người đang đứng đầu Queue
    if (it->second.front() != reservationId)
        return false;

    // Xóa khỏi Queue
    it->second.pop();

    // WAITING -> SERVED
    reservation->status = "SERVED";

    return true;
}


