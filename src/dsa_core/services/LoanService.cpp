#include "LoanService.h"

ReturnReceipt LoanService::returnBook(const string& loanId, const Date& returnDate, const string& quality) {
    ReturnReceipt receipt;
    receipt.loanId = loanId;
    receipt.isSuccess = false;
    receipt.lateDays = 0;
    receipt.lateFee = 0.0;
    receipt.damageFee = 0.0;
    receipt.totalFee = 0.0;

    // 1. Tra cứu phiếu mượn trên Bảng băm: O(1) trung bình
    Loan* loan = loanRepo.findById(loanId);
    if (!loan) {
        receipt.message = "Lỗi: Không tìm thấy mã phiếu mượn.";
        return receipt;
    }
    if (loan->status == "RETURNED") {
        receipt.message = "Lỗi: Phiếu mượn đã được hoàn tất trước đó.";
        return receipt;
    }

    // 2. Tra cứu đầu sách trên Bảng băm: O(1) trung bình
    Book* book = bookRepo.findById(loan->bookId);
    if (!book) {
        receipt.message = "Lỗi: Dữ liệu sách liên kết không tồn tại.";
        return receipt;
    }

    // 3. Tính phạt trễ: 10.000 đ/ngày
    int lateDays = 0;
    if (returnDate > loan->dueDate) {
        lateDays = returnDate.toDays() - loan->dueDate.toDays();
    }
    double lateFee = lateDays * 10000.0;

    // 4. Tính phạt hư hại
    double damageFee = 0.0;
    if (quality == "Hu hong nhe") {
        damageFee = 20000.0;
    } else if (quality == "Hu hong nang") {
        damageFee = book->price;
    }
    double totalFee = lateFee + damageFee;

    // 5. Cập nhật tại chỗ (In-place Mutation): O(1)
    loan->returnDate = returnDate;
    loan->status = "RETURNED";

    // 6. Cập nhật kho sách
    if (quality != "Hu hong nang") {
        book->available += 1;
    }

    // 7. Ghi nhận phiếu phạt nếu có
    if (totalFee > 0) {
        Fine* fine = new Fine();
        fine->fineId = "F_" + loanId;
        fine->loanId = loanId;
        fine->memberId = loan->memberId;
        fine->amount = totalFee;
        fine->reason = (lateDays > 0 ? "Tre " + to_string(lateDays) + " ngay. " : "") +
                       (damageFee > 0 ? "Hu hai: " + quality : "");
        fine->status = "UNPAID";
        fineRepo.add(fine);
    }

    receipt.isSuccess = true;
    receipt.lateDays = lateDays;
    receipt.lateFee = lateFee;
    receipt.damageFee = damageFee;
    receipt.totalFee = totalFee;
    receipt.message = "Xử lý trả sách thành công.";
    return receipt;
}
