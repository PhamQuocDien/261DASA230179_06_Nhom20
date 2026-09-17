#include "MemberRepository.h"
MemberRepository::MemberRepository() {}
std::vector<Member>& MemberRepository::getAll() {
    return members;
}
const std::vector<Member>& MemberRepository::getAll() const {
    return members;
}
Member* MemberRepository::findById(const std::string& memberId) {
    for (Member& member : members) {
        if (member.memberId == memberId) {
            return &member;
        }
    }
    return nullptr;
}
const Member* MemberRepository::findById(const std::string& memberId) const {
    for (const Member& member : members) {
        if (member.memberId == memberId) {
            return &member;
        }
    }
    return nullptr;
}
bool MemberRepository::add(const Member& member) {
    if (findById(member.memberId) != nullptr) {
        return false;
    }
    members.push_back(member);
    return true;
}
bool MemberRepository::update(const Member& member) {
    Member* existingMember = findById(member.memberId);
    if (existingMember == nullptr) {
        return false;
    }
    *existingMember = member;
    return true;
}
bool MemberRepository::removeById(const std::string& memberId) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->memberId == memberId) {
            members.erase(it);
            return true;
        }
    }
    return false;
}