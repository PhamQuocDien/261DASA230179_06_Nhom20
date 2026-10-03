#include "AdminService.h"

#include <cctype>

using namespace std;

AdminService::AdminService(
    FineRepository& fineRepository
)
    : fineRepository(fineRepository)
{
}

bool AdminService::authenticate(
    const std::string& idAdmin,
    const std::string& password
) const
{
    return
        idAdmin == "QuanLyAdmin" &&
        password == "QuanLyThuVien";
}


// =====================================================
// XÁC NHẬN ADMIN ĐÃ TRẢ TIỀN PHẠT
// =====================================================
//
// Toàn bộ quyết định nằm ở C++:
//   1. Xác thực Admin_ID + mật khẩu
//   2. Kiểm tra Fine có tồn tại không
//   3. UNPAID -> PAID, không đổi số tiền
// Frontend chỉ gửi request và hiển thị kết quả.
//

PayFineResult AdminService::payFine(
    const string& fineId,
    const string& idAdmin,
    const string& password
) {
    PayFineResult result{};
    result.isSuccess = false;
    result.fineId = fineId;

    if (fineId.empty()) {
        result.message =
            "Thieu Fine_ID.";

        return result;
    }

    // 1. Xác thực Admin
    if (!authenticate(idAdmin, password)) {
        result.message =
            "ID quan ly hoac mat khau khong dung.";

        return result;
    }

    // 2. Tìm Fine
    Fine* fine =
        fineRepository.findById(fineId);

    if (fine == nullptr) {
        result.message =
            "Loi: Khong tim thay tien phat voi Fine_ID nay.";

        return result;
    }

    // Phòng thủ: chấp nhận cả chữ thường và IN HOA
    string currentStatus = fine->status;

    for (char& c : currentStatus) {
        c = static_cast<char>(
            toupper(static_cast<unsigned char>(c))
        );
    }

    // 3. Fine đã PAID -> không xử lý lại
    if (currentStatus == "PAID") {
        result.status = fine->status;
        result.message =
            "Tien phat nay da duoc thanh toan truoc do.";

        return result;
    }

    // UNPAID -> PAID, giữ nguyên amount / loanId / memberId
    Fine paidFine = *fine;
    paidFine.status = "PAID";

    if (!fineRepository.update(paidFine)) {
        result.message =
            "Loi: Khong the cap nhat tien phat trong he thong.";

        return result;
    }

    result.isSuccess = true;
    result.status = paidFine.status;
    result.message =
        "Da xac nhan thanh toan tien phat. "
        "Fine_ID: " + fineId;

    return result;
}