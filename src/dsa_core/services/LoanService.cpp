#include "LoanService.h"

ReturnReceipt LoanService::returnBook(const string& loanId, const Date& returnDate, const string& quality) {
    ReturnReceipt receipt{};
    receipt.loanId = loanId;

    Loan* loan = loanRepo.findById(loanId);
    if (!loan) {
        receipt.message = "Loi: Khong tim thay ma phieu muon.";
        return receipt;
    }
    if (loan->status == "RETURNED") {
        receipt.message = "Loi: Phieu muon da duoc tra truoc do.";
        return receipt;
    }
    if (returnDate.year <= 0 || returnDate.month <= 0 || returnDate.day <= 0) {
        receipt.message = "Loi: Ngay tra khong hop le.";
        return receipt;
    }

    Book* book = nullptr;
    for (Book& candidate : bookRepo.getAll()) {
        for (const BookCopy& copy : candidate.copies) {
            if (copy.bookId == loan->bookId) {
                book = &candidate;
                break;
            }
        }
        if (book != nullptr) break;
    }
    if (!book) {
        receipt.message = "Loi: Du lieu sach lien ket khong ton tai.";
        return receipt;
    }

    const Date dueDate = Date::parse(loan->dueDate);
    if (dueDate.year <= 0 || dueDate.month <= 0 || dueDate.day <= 0) {
        receipt.message = "Loi: Han tra cua phieu muon khong hop le.";
        return receipt;
    }
    if (returnDate < dueDate) {
 
        receipt.lateDays = 0;
    } else if (returnDate > dueDate) {
        receipt.lateDays = returnDate.toDays() - dueDate.toDays();
    }


    receipt.lateFee = receipt.lateDays * 10000.0;

    if (quality == "Tot") {
        receipt.damageFee = 0.0;
    } else if (quality == "Hu hong nhe") {
        receipt.damageFee = 20000.0;
    } else if (quality == "Hu hong nang") {
        receipt.damageFee = 100000.0;
    } else {
        receipt.message = "Loi: Tinh trang sach khong hop le.";
        return receipt;
    }

    receipt.totalFee = receipt.lateFee + receipt.damageFee;


    loan->returnDate = returnDate.toString();
    loan->status = "RETURNED";


    if (quality != "Hu hong nang") {
        bool restored = false;
        for (BookCopy& copy : book->copies) {
            if (copy.bookId == loan->bookId) {
                if (copy.status == "BORROWED") {
                    copy.status = "AVAILABLE";
                    restored = true;
                }
                break;
            }
        }
        if (!restored) {

            for (BookCopy& copy : book->copies) {
                if (copy.bookId == loan->bookId) {
                    copy.status = "AVAILABLE";
                    break;
                }
            }
        }
    } else {
        for (BookCopy& copy : book->copies) {
            if (copy.bookId == loan->bookId) {
                copy.status = "DAMAGED";
                break;
            }
        }
    }

    if (receipt.totalFee > 0.0) {
        Fine fine;
        fine.fineId = "F_" + loanId;
        fine.loanId = loanId;
        fine.memberId = loan->memberId;
        fine.amount = receipt.totalFee;
        fine.reason = (receipt.lateDays > 0
            ? "Tre " + to_string(receipt.lateDays) + " ngay. "
            : "") +
            (receipt.damageFee > 0
                ? "Hu hai: " + quality
                : "");
        fine.status = "UNPAID";

        Fine* existing = fineRepo.findByLoanId(loanId);
        if (existing != nullptr) {
            *existing = fine;
        } else {
            fineRepo.add(fine);
        }
    }
    receipt.isSuccess = true;
    receipt.message = "Xu ly tra sach thanh cong.";
    return receipt;
}
