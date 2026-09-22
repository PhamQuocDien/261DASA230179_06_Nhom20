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


bool ReservationService::enqueue(const string& memberId,const string& bookId)
{
    //1. Kiểm tra Member có tồn tại không 
    Member* member = memberRepository.findById(memberId);
    if (member == nullptr)
        return false;

    // 2. Kiểm tra Book có tồn tại không
    Book* book = bookRepository.findByCode(bookId);
    if (book == nullptr)
        return false;

    // 3. Chỉ cho đăng ký chờ khi tất cả bản sách của đầu sách đều đã hết
    bool hasAvailableCopy = false;

    for (const BookCopy& copy : book->copies) {
        if (copy.status == "available") {
            hasAvailableCopy = true;
            break;
        }
    }

    if (hasAvailableCopy)
        return false;
    
    // 4. Kiểm tra Member có đang mượn sách này không
    Loan* loan = loanRepository.findByMemberAndBook(memberId, bookId);
    if (loan != nullptr && loan->status=="BORROWING")
        return false;

    // 5. Kiểm tra Member đã có yêu cầu WAITING cho Book này chưa
    for (const auto& reservation : reservationRepository.getAll())
    {
        if (reservation.memberId == memberId && reservation.bookId == bookId && reservation.status == "WAITING") {
            return false;
        }
       
    }
    // 6. Tạo Reservation mới
    string reservationId = "R" + to_string(nextReservationId++);

    Reservation r;
    r.reservationId = reservationId;
    r.memberId = memberId;
    r.bookId = bookId;
    r.reservationDate = getCurrentTime();
    r.status = "WAITING";

    // 7. Lưu Reservation vào Repository
    reservationRepository.add(r);


    // 8. Đưa Reservation vào Queue theo thứ tự đăng ký
    waitQueues[bookId].push(reservationId);


    return true;
}
bool ReservationService::cancel(const string& reservationId)
{
    // 1. Tìm yêu cầu chờ theo Reservation_ID
    Reservation* reservation = reservationRepository.findById(reservationId);

    // Không tồn tại yêu cầu
    if (reservation==nullptr)
        return false;
    // Chỉ yêu cầu đang WAITING mới được hủy
    if (reservation->status != "WAITING")
        return false;

    // Chuyển trạng thái sang CANCELLED
   // Request vẫn nằm trong Queue và sẽ được next() bỏ qua
    reservation->status = "CANCELLED";

    return true;
}
Reservation* ReservationService::next(const string& bookId)
{
    // 1. Tìm Queue tương ứng với Book_ID
    auto it = waitQueues.find(bookId);
    if (it == waitQueues.end())
        return nullptr;
    if (it->second.empty() == true)
        return nullptr;

    // 2. Duyệt từ đầu Queue theo nguyên tắc FIFO
    while (!it->second.empty())
    {
        // Queue lưu Reservation_ID để phân biệt từng lần đăng ký
        string reservationId = it->second.front();

        // 3. Tìm Reservation tương ứng
        Reservation* reservation = reservationRepository.findById(reservationId);

        // 4. Request không tồn tại hoặc không còn WAITING
      //    → bỏ qua và xét request tiếp theo
        if (reservation == nullptr|| reservation->status!="WAITING") {
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
