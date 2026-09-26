
#include "MemberRepository.h"
MemberRepository::MemberRepository() {}
// Lấy toàn bộ danh sách thành viên
std::vector<Member>& MemberRepository::getAll() {
    return members;
}
// Đọc danh sách
const std::vector<Member>& MemberRepository::getAll() const {
    return members;
}
// Tìm thành viên theo ID
Member* MemberRepository::findById(const std::string& memberId) {
    for (Member& member : members) {
        if (member.memberId == memberId) {
            return &member;
        }
    }

    return nullptr; 
}
// Tìm thành viên theo ID - phiên bản chỉ đọc (const)
const Member* MemberRepository::findById(const std::string& memberId) const {
    for (const Member& member : members) {
        if (member.memberId == memberId) {
            return &member; 
        }
    }

    return nullptr; 
}
// Thêm thành viên mới
bool MemberRepository::add(const Member& member) {
    if (findById(member.memberId) != nullptr) {
        return false;
    }
    members.push_back(member); 
    return true;
}
// Cập nhật thông tin thành viên
bool MemberRepository::update(const Member& member) {
    Member* existingMember = findById(member.memberId);
    if (existingMember == nullptr) {
        return false;
    }
    *existingMember = member;
    return true;
}
// Xóa thành viên theo ID
bool MemberRepository::removeById(const std::string& memberId) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->memberId == memberId) {
            members.erase(it);
            return true;
        }
    }
    return false;
}
