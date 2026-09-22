#include "ReservationService.h"

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
}

bool ReservationService::enqueue(const string& memberId, const string& bookCode)
{
    // 1. Kiểm tra Member có tồn tại không
    Member* member = memberRepository.findById(memberId);
    if (member == nullptr)
        return false;

    // 2. Kiểm tra đầu sách có tồn tại không
    Book* book = bookRepository.findByCode(bookCode);
    if (book == nullptr)
        return false;

    // 3. Chỉ cho đăng ký chờ khi tất cả bản sách của đầu sách đều đã hết
    bool hasAvailableCopy = false;

    for (const BookCopy& copy : book->copies)
    {
        if (copy.status == "available")
        {
            hasAvailableCopy = true;
            break;
        }
    }

    // Nếu vẫn còn ít nhất một bản có thể mượn
    // thì không cần đăng ký chờ
    if (hasAvailableCopy)
        return false;

    // 4. Kiểm tra Member có đang mượn đầu sách này không
    Loan* loan = loanRepository.findByMemberAndBook(memberId, bookCode);

    if (loan != nullptr && loan->status == "BORROWING")
        return false;

    // 5. Kiểm tra Member đã có yêu cầu WAITING cho đầu sách này chưa
    for (const auto& reservation : reservationRepository.getAll())
    {
        if (reservation.memberId == memberId &&
            reservation.bookCode == bookCode &&
            reservation.status == "WAITING")
        {
            return false;
        }
    }

    // 6. Tạo Reservation mới
    string reservationId = "R" + to_string(nextReservationId++);

    Reservation r;
    r.reservationId = reservationId;
    r.memberId = memberId;
    r.bookCode = bookCode;
    r.reservationDate = getCurrentTime();
    r.status = "WAITING";

    // 7. Lưu Reservation vào Repository
    reservationRepository.add(r);

    // 8. Đưa Reservation vào Queue theo thứ tự đăng ký
    waitQueues[bookCode].push(reservationId);

    return true;
}

bool ReservationService::cancel(const string& reservationId)
{
    // 1. Tìm yêu cầu chờ theo Reservation_ID
    Reservation* reservation =
        reservationRepository.findById(reservationId);

    // Không tồn tại yêu cầu
    if (reservation == nullptr)
        return false;

    // 2. Chỉ yêu cầu đang WAITING mới được hủy
    if (reservation->status != "WAITING")
        return false;

    // 3. Chuyển trạng thái sang CANCELLED
    // Request vẫn nằm trong Queue
    // và sẽ được next() bỏ qua
    reservation->status = "CANCELLED";

    return true;
}

Reservation* ReservationService::next(const string& bookCode)
{
    // 1. Tìm Queue tương ứng với mã đầu sách
    auto it = waitQueues.find(bookCode);

    if (it == waitQueues.end())
        return nullptr;

    if (it->second.empty())
        return nullptr;

    // 2. Duyệt từ đầu Queue theo nguyên tắc FIFO
    while (!it->second.empty())
    {
        // Queue lưu Reservation_ID để phân biệt từng lần đăng ký
        string reservationId = it->second.front();

        // 3. Tìm Reservation tương ứng
        Reservation* reservation =
            reservationRepository.findById(reservationId);

        // 4. Request không tồn tại hoặc không còn WAITING
        //    → bỏ qua và xét Request tiếp theo
        if (reservation == nullptr ||
            reservation->status != "WAITING")
        {
            it->second.pop();
            continue;
        }

        // 5. Request hợp lệ đầu tiên → phục vụ
        it->second.pop();

        reservation->status = "SERVED";

        return reservation;
    }

    return nullptr;
}
