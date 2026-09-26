#include "MemberService.h"

using namespace std;

// Nhận MemberRepository để xử lý dữ liệu thành viên
MemberService::MemberService(
    MemberRepository& repository
)
    : repository(repository) {
}

// =====================================
// THÊM THÀNH VIÊN
// =====================================

bool MemberService::addMember(
    Member& member
) {
    // Kiểm tra thông tin bắt buộc
    if (member.name.empty()) {
        return false;
    }
    if (member.email.empty()) {
        return false;
    }
    if (member.phone.empty()) {
        return false;
    }

    // =================================
    // TỰ SINH MEMBER_ID
    // Dạng: M001, M002, M003, ...
    // =================================

    // Lấy số thứ tự tiếp theo
    int nextNumber =
        static_cast<int>(
            repository.getAll().size()
            ) + 1;
    string newMemberId;


    // Tìm ID chưa được sử dụng
    while (true) {
        newMemberId = "M";

        // Thêm số 0 để tạo dạng M001, M002...
        if (nextNumber < 10) {
            newMemberId += "00";
        }
        else if (nextNumber < 100) {
            newMemberId += "0";
        }

        // Ghép số thứ tự vào ID
        newMemberId +=
            to_string(nextNumber);

        // Kiểm tra ID đã tồn tại chưa
        if (
            repository.findById(
                newMemberId
            ) == nullptr
            ) {
            break;
        }

        // Nếu bị trùng thì tăng số lên
        nextNumber++;
    }


    // Gán ID tự sinh cho thành viên
    member.memberId = newMemberId;

    // Nếu chưa có trạng thái thì mặc định ACTIVE
    if (member.status.empty()) {
        member.status = "ACTIVE";
    }

    // Gọi Repository để thêm thành viên
    return repository.add(member);
}

// =====================================
// LẤY TẤT CẢ THÀNH VIÊN
// =====================================

const vector<Member>&
MemberService::getAllMembers() const {

    // Trả về danh sách thành viên
    return repository.getAll();
}

// =====================================
// TÌM THÀNH VIÊN THEO ID
// =====================================

Member* MemberService::getMemberById(
    const string& memberId
) {

    // Không cho phép tìm với ID rỗng
    if (memberId.empty()) {
        return nullptr;
    }
    // Gọi Repository để tìm thành viên
    return repository.findById(memberId);
}


// =====================================
// CẬP NHẬT THÀNH VIÊN
// =====================================

bool MemberService::updateMember(
    const Member& member
) {

    // Kiểm tra các thông tin bắt buộc
    if (member.memberId.empty()) {
        return false;
    }
    if (member.name.empty()) {
        return false;
    }
    if (member.email.empty()) {
        return false;
    }
    if (member.phone.empty()) {
        return false;
    }

    // Tạo bản sao để cập nhật
    Member updatedMember = member;

    // Nếu chưa có status thì mặc định ACTIVE
    if (updatedMember.status.empty()) {
        updatedMember.status = "ACTIVE";
    }

    // Gửi dữ liệu sang Repository để cập nhật
    return repository.update(updatedMember);
}

// =====================================
// XÓA THÀNH VIÊN
// =====================================

bool MemberService::deleteMember(
    const string& memberId
) {
    // Không cho phép xóa với ID rỗng
    if (memberId.empty()) {
        return false;
    }

    return repository.removeById(memberId);
}
