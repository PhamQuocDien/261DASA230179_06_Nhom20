#include "RenewService.h"

#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

RenewService::RenewService(
    LoanRepository& lr,
    BookRepository& br,
    ReservationRepository& rr
)
    : loanRepo(lr), bookRepo(br), reservationRepo(rr) {
}


RenewResult RenewService::renewBook(const string& loanId) {
    RenewResult result;
    result.isSuccess = false;

    //Kiểm tra Loan_ID có tồn tại không
    Loan* loan = loanRepo.findById(loanId);
    if (loan == nullptr) {
        result.message = "Loi: Khong tim thay phieu muon voi Loan_ID nay.";
        return result;
    }

    //Kiểm tra trạng thái phiếu mượn
    if (loan->status != "borrowing" && loan->status != "BORROWING") {
        result.message = "Loi: Phieu muon nay da duoc tra hoac khong o trang thai dang muon.";
        return result;
    }

    // Kiểm tra số lần gia hạn
    if (loan->renewalCount != 0) {
        result.message = "Loi: Moi phieu muon chi duoc gia han toi da 1 lan.";
        return result;
    }

    // Kiểm tra ngày gia hạn
    time_t now = time(nullptr);
    tm localTime{};
#ifdef _WIN32
    localtime_s(&localTime, &now);
#else
    localtime_r(&now, &localTime);
#endif
    char buffer[20];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", &localTime);
    Date today = Date::parse(string(buffer));

    Date dueDate = Date::parse(loan->dueDate);

    if (today > dueDate) {
        result.message = "Loi: Khong the gia han vi sach da qua han tra.";
        return result;
    }

    //Tìm đầu sách để kiểm tra hàng chờ
    Book* book = nullptr;
    for (Book& candidate : bookRepo.getAll()) {
        for (const BookCopy& copy : candidate.copies) {
            if (copy.bookId == loan->bookId) {
                book = &candidate;
                break;
            }
        }
        if (book != nullptr) {
            break;
        }
    }

    if (book == nullptr) {
        result.message = "Loi: Khong tim thay du lieu sach lien ket voi phieu muon.";
        return result;
    }

    //Kiểm tra có ai đang đăng ký chờ không
    for (const Reservation& res : reservationRepo.getAll()) {
        if (res.bookCode == book->bookCode && res.status == "WAITING") {
            result.message = "Loi: Khong the gia han vi sach dang co nguoi dang ky cho muon.";
            return result;
        }
    }

    // Cộng thêm 7 ngày
    const int RENEW_DAYS = 7;
    string newDueDate = LoanService::calculateDueDate(loan->dueDate, RENEW_DAYS);

    //Cập nhật thông tin phiếu mượn
    loan->dueDate = newDueDate;
    loan->renewalCount += 1;

    //Lưu thay đổi vào repository
    bool updated = loanRepo.update(*loan);

    if (!updated) {
        result.message = "Loi: Khong the cap nhat phieu muon trong he thong.";
        return result;
    }

    //Trả về kết quả thành công
    result.isSuccess = true;
    result.message = "Gia han thanh cong! Han tra moi la: " + newDueDate;
    result.loan = *loan;

    return result;
}
