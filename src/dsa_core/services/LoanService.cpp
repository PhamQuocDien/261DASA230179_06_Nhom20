#include "LoanService.h"
using namespace std;

// =====================================================
// XỬ LÝ TRẢ SÁCH
// =====================================================

ReturnReceipt LoanService::returnBook(
    const string& loanId,
    const Date& returnDate,
    const string& quality
) {
    ReturnReceipt receipt{};
    receipt.loanId = loanId;

    // Tìm phiếu mượn theo mã
    Loan* loan = loanRepo.findById(loanId);

    // Không tìm thấy phiếu mượn
    if (!loan) {
        receipt.message = "Loi: Khong tim thay ma phieu muon.";
        return receipt;
    }

    // Kiểm tra phiếu đã được trả trước đó chưa
    if (loan->status == "RETURNED") {
        receipt.message = "Loi: Phieu muon da duoc tra truoc do.";
        return receipt;
    }

    // Kiểm tra ngày trả hợp lệ
    if (returnDate.year <= 0 ||
        returnDate.month <= 0 ||
        returnDate.day <= 0) {
        receipt.message = "Loi: Ngay tra khong hop le.";
        return receipt;
    }

    // =================================================
    // TÌM ĐẦU SÁCH CHỨA BẢN COPY ĐƯỢC MƯỢN
    // =================================================

    Book* book = nullptr;
    for (Book& candidate : bookRepo.getAll()) {
        for (const BookCopy& copy : candidate.copies) {
            // Tìm bản sách có bookId trùng với phiếu mượn
            if (copy.bookId == loan->bookId) {
                book = &candidate;
                break;
            }
        }
        if (book != nullptr)
            break;
    }

    // Không tìm thấy dữ liệu sách liên kết
    if (!book) {
        receipt.message =
            "Loi: Du lieu sach lien ket khong ton tai.";
        return receipt;
    }


    // =================================================
    // TÍNH SỐ NGÀY TRỄ
    // =================================================

    // Lấy ngày hết hạn từ phiếu mượn
    const Date dueDate = Date::parse(loan->dueDate);

    // Kiểm tra hạn trả có hợp lệ không
    if (dueDate.year <= 0 ||
        dueDate.month <= 0 ||
        dueDate.day <= 0) {
        receipt.message =
            "Loi: Han tra cua phieu muon khong hop le.";
        return receipt;
    }

    // Trả trước hoặc đúng hạn
    if (returnDate < dueDate) {
        receipt.lateDays = 0;
    }
    // Trả trễ hạn
    else if (returnDate > dueDate) {
        receipt.lateDays =
            returnDate.toDays() - dueDate.toDays();
    }


    // Phạt 10.000 VNĐ cho mỗi ngày trễ
    receipt.lateFee =
        receipt.lateDays * 10000.0;

    // =================================================
    // TÍNH PHÍ HƯ HỎNG
    // =================================================

    // Sách tốt
    if (quality == "Tot") {
        receipt.damageFee = 0.0;
    }

    // Hư hỏng nhẹ
    else if (quality == "Hu hong nhe") {
        receipt.damageFee = 20000.0;
    }

    // Hư hỏng nặng
    else if (quality == "Hu hong nang") {
        receipt.damageFee = 100000.0;
    }

    // Tình trạng không hợp lệ
    else {
        receipt.message =
            "Loi: Tinh trang sach khong hop le.";
        return receipt;
    }

    // Tổng tiền phạt = phạt trễ + phí hư hỏng
    receipt.totalFee =
        receipt.lateFee + receipt.damageFee;

    // =================================================
    // CẬP NHẬT PHIẾU MƯỢN
    // =================================================

    // Lưu ngày trả thực tế
    loan->returnDate = returnDate.toString();

    // Đổi trạng thái phiếu thành đã trả
    loan->status = "RETURNED";

    // =================================================
    // CẬP NHẬT TRẠNG THÁI SÁCH
    // =================================================

    // Nếu không hư hỏng nặng
    if (quality != "Hu hong nang") {
        bool restored = false;
        for (BookCopy& copy : book->copies) {
            if (copy.bookId == loan->bookId) {
                // Nếu sách đang được mượn
                if (copy.status == "BORROWED") {
                    // Trả sách về trạng thái có thể mượn
                    copy.status = "AVAILABLE";
                    restored = true;
                }
                break;
            }
        }

        // Trường hợp trạng thái trước đó không đúng
        // vẫn đưa bản sách về AVAILABLE
        if (!restored) {
            for (BookCopy& copy : book->copies) {
                if (copy.bookId == loan->bookId) {
                    copy.status = "AVAILABLE";
                    break;
                }
            }
        }
    }
    // Nếu sách hư hỏng nặng
    else {
        for (BookCopy& copy : book->copies) {
            if (copy.bookId == loan->bookId) {
                // Đánh dấu sách bị hư
                copy.status = "DAMAGED";
                break;
            }
        }
    }


    // =================================================
    // TẠO / CẬP NHẬT TIỀN PHẠT
    // =================================================

    // Chỉ tạo tiền phạt nếu tổng phí > 0
    if (receipt.totalFee > 0.0) {
        Fine fine;

        // Tạo mã tiền phạt từ mã phiếu mượn
        fine.fineId = "F_" + loanId;
        fine.loanId = loanId;
        fine.memberId = loan->memberId;

        // Lưu số tiền phạt
        fine.amount = receipt.totalFee;

        // Tạo lý do tiền phạt
        fine.reason = "";

        // Nếu trả trễ thì thêm lý do trễ
        if (receipt.lateDays > 0) {
            fine.reason =
                "Tre " +
                to_string(receipt.lateDays) +
                " ngay. ";
        }
        // Nếu sách bị hư thì thêm lý do hư hỏng
        if (receipt.damageFee > 0) {
            fine.reason +=
                "Hu hai: " + quality;
        }

        // Ban đầu tiền phạt chưa thanh toán
        fine.status = "UNPAID";

        // Kiểm tra đã có tiền phạt cho phiếu này chưa
        Fine* existing =
            fineRepo.findByLoanId(loanId);
        if (existing != nullptr) {

            // Nếu đã có thì cập nhật
            *existing = fine;
        }
        else {
            // Nếu chưa có thì thêm mới
            fineRepo.add(fine);
        }
    }

    // =================================================
    // TRẢ KẾT QUẢ
    // =================================================

    receipt.isSuccess = true;
    receipt.message =
        "Xu ly tra sach thanh cong.";
    return receipt;
}

string LoanService::generateLoanId() {
    int count =
        loanRepo.getAll().size() + 1;
    string id = "L";

    if (count < 10)
        id += "00";
    else if (count < 100)
        id += "0";

    id += to_string(count);

    while (loanRepo.findById(id) != nullptr) {
        count++;
        id =
            "L" +
            string(
                count < 10
                ? "00"
                : (count < 100 ? "0" : "")
            ) +
            to_string(count);
    }
    return id;
}

string LoanService::calculateDueDate(
    const string& borrowDateStr,
    int daysToAdd
) {
    Date d = Date::parse(borrowDateStr);
    d.day += daysToAdd;

    while (true) {
        int daysInMonth = 31;

        if (d.month == 4 ||
            d.month == 6 ||
            d.month == 9 ||
            d.month == 11) {
            daysInMonth = 30;
        }

        else if (d.month == 2) {
            bool isLeapYear =
                (d.year % 4 == 0 &&
                    d.year % 100 != 0) ||
                (d.year % 400 == 0);
            daysInMonth =
                isLeapYear ? 29 : 28;
        }

        if (d.day <= daysInMonth) {
            break;
        }

        d.day -= daysInMonth;
        d.month++;

        if (d.month > 12) {
            d.month = 1;
            d.year++;
        }
    }
    return d.toString();
}

BorrowResult LoanService::borrowBook(
    const string& memberId,
    const string& bookCode,
    const string& borrowDateStr,
    MemberRepository& memberRepo
) {
    BorrowResult result;
    result.isSuccess = false;

    if (memberId.empty() ||
        bookCode.empty() ||
        borrowDateStr.empty()) {
        result.message =
            "Du lieu sai: Vui long nhap du thong tin.";
        return result;
    }

    const Member* member =
        memberRepo.findById(memberId);

    if (member == nullptr) {
        result.message =
            "Member sai: Khong tim thay the doc gia "
            + memberId;
        return result;
    }

    if (member->status != "ACTIVE") {
        result.message =
            "Doc gia dang bi khoa the, khong the muon.";
        return result;
    }

    Book* book =
        bookRepo.findByCode(bookCode);
    if (book == nullptr) {
        result.message =
            "Du lieu sai: Dau sach "
            + bookCode
            + " khong ton tai.";
        return result;
    }

    string availableBookId = "";
    for (auto& copy : book->copies) {
        if (copy.status == "available") {
            availableBookId =
                copy.bookId;
            break;
        }
    }

    if (availableBookId.empty()) {

        result.message =
            "Het sach: Toan bo sach "
            + bookCode
            + " da duoc muon.";

        return result;
    }

    int activeLoans = 0;
    bool alreadyBorrowingThisBook = false;
    const auto& allLoans =
        loanRepo.getAll();
    for (auto it = allLoans.rbegin();
        it != allLoans.rend();
        ++it) {
        if (it->memberId == memberId &&
            it->status == "borrowing") {
            activeLoans++;
            for (const auto& copy : book->copies) {
                if (it->bookId == copy.bookId) {
                    alreadyBorrowingThisBook = true;
                    break;
                }
            }
            if (alreadyBorrowingThisBook ||
                activeLoans >= 10) {
                break;
            }
        }
    }

    if (alreadyBorrowingThisBook) {
        result.message =
            "Dang muon: Doc gia dang giu mot cuon "
            "cua dau sach nay roi.";
        return result;
    }

    if (activeLoans >= 10) {
        result.message =
            "Dang muon: Da dat gioi han muon "
            "10 cuon sach, khong the muon them.";
        return result;
    }

    Loan newLoan;
    newLoan.loanId =
        generateLoanId();
    newLoan.memberId =
        memberId;
    newLoan.bookId =
        availableBookId;
    newLoan.borrowDate =
        borrowDateStr;
    newLoan.dueDate =
        calculateDueDate(
            borrowDateStr,
            14
        );
    newLoan.returnDate = "";
    newLoan.renewalCount = 0;
    newLoan.status = "borrowing";

    for (auto& copy : book->copies) {
        if (copy.bookId ==
            availableBookId) {
            copy.status = "borrowed";
            break;
        }
    }
    bookRepo.update(*book);
    loanRepo.add(newLoan);
    result.isSuccess = true;
    result.message =
        "Muon sach thanh cong! Bien lai: "
        + newLoan.loanId;
    result.loan =
        newLoan;
    return result;
}
