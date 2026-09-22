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


string LoanService::generateLoanId() {
    int count = loanRepo.getAll().size() + 1;
    string id = "L";
    if (count < 10) id += "00";
    else if (count < 100) id += "0";
    id += to_string(count);
    
    while(loanRepo.findById(id) != nullptr) {
        count++;
        id = "L" + string(count < 10 ? "00" : (count < 100 ? "0" : "")) + to_string(count);
    }
    return id;
}


// tính ngày trả sách đúng với ngày/tháng/năm chuẩn
string LoanService::calculateDueDate(const string& borrowDateStr, int daysToAdd) {
    Date d = Date::parse(borrowDateStr);
    d.day += daysToAdd;

    while (true) {
        // tìm số ngày trong tháng này
        int daysInMonth = 31; 
        if (d.month == 4 || d.month == 6 || d.month == 9 || d.month == 11) {
            daysInMonth = 30;
        } else if (d.month == 2) {
            bool isLeapYear = (d.year % 4 == 0 && d.year % 100 != 0) || (d.year % 400 == 0);
            daysInMonth = isLeapYear ? 29 : 28;
        }
        
        if (d.day <= daysInMonth) {
            break;
        }
        
        // cập nhật ngày/tháng/năm nếu hạn trả trượt sang tháng sau
        d.day -= daysInMonth;
        d.month++;

        if (d.month > 12) {
            d.month = 1;
            d.year++;
        }
    }
    
    return d.toString();
}

BorrowResult LoanService::borrowBook(const string& memberId, const string& bookCode, const string& borrowDateStr, MemberRepository& memberRepo) {
    BorrowResult result;
    result.isSuccess = false;

    // 1. Kiểm tra dữ liệu bị để trống
    if (memberId.empty() || bookCode.empty() || borrowDateStr.empty()) {
        result.message = "Du lieu sai: Vui long nhap du thong tin.";
        return result;
    }

    // 2. Kiểm tra mã độc giả
    const Member* member = memberRepo.findById(memberId);
    if (member == nullptr) {
        result.message = "Member sai: Khong tim thay the doc gia " + memberId;
        return result;
    }
    if (member->status != "ACTIVE") {
        result.message = "Doc gia dang bi khoa the, khong the muon.";
        return result;
    }

    // 3. Kiểm tra sách 
    Book* book = bookRepo.findByCode(bookCode);
    if (book == nullptr) {
        result.message = "Du lieu sai: Dau sach " + bookCode + " khong ton tai.";
        return result;
    }

    string availableBookId = "";
    for (auto& copy : book->copies) {
        if (copy.status == "AVAILABLE" || copy.status == "available") { 
            availableBookId = copy.bookId;
            break;
        }
    }

    if (availableBookId.empty()) {
        result.message = "Het sach: Toan bo sach " + bookCode + " da duoc muon.";
        return result;
    }

    // 4. Kiểm tra giới hạn mượn
    int activeLoans = 0;
    bool alreadyBorrowingThisBook = false;
    
    for (const auto& copy : book->copies) {
        const Loan* existing = loanRepo.findByMemberAndBook(memberId, copy.bookId);
        if (existing != nullptr && (existing->status == "BORROWING" || existing->status == "borrowing")) {
            alreadyBorrowingThisBook = true;
            break;
        }
    }

    if (alreadyBorrowingThisBook) {
        result.message = "Dang muon: Doc gia dang giu mot cuon cua dau sach nay roi.";
        return result;
    }

    //Đếm số sách đang mượn
    for (const Loan& loan : loanRepo.getAll()) {
        if (loan.memberId == memberId && (loan.status == "BORROWING" || loan.status == "borrowing")) {
            activeLoans++;
        }
    }

    // Huỷ yêu cầu mượn nếu đã đạt giới hạn mượn
    if (activeLoans >= 3) {
        result.message = "Dang muon: Da dat gioi han muon 3 cuon sach.";
        return result;
    }

    // 5. Tạo phiếu Loan mới
    Loan newLoan;
    newLoan.loanId = generateLoanId();
    newLoan.memberId = memberId;
    newLoan.bookId = availableBookId;
    newLoan.borrowDate = borrowDateStr;
    newLoan.dueDate = calculateDueDate(borrowDateStr, 14);
    newLoan.returnDate = "";
    newLoan.renewalCount = 0;
    newLoan.status = "BORROWING";

    // 6. Cập nhật trạng thái sách vừa mượn thành BORROWED
    for (auto& copy : book->copies) {
        if (copy.bookId == availableBookId) {
            copy.status = "BORROWED";
            break;
        }
    }
    
    bookRepo.update(*book);
    loanRepo.add(newLoan);

    result.isSuccess = true;
    result.message = "Muon sach thanh cong! Bien lai: " + newLoan.loanId;
    result.loan = newLoan;

    return result;
}
