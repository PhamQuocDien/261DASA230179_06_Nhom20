
#include "MemberService.h"


MemberService::MemberService(
    MemberRepository& repository
)
    : repository(repository) {
}


// =====================================
// THEM THANH VIEN
// =====================================

bool MemberService::addMember(
    Member& member
) {

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
    // TU SINH MEMBER_ID
    // Dang: M001, M002, M003, ...
    // =================================

    int nextNumber =
        static_cast<int>(
            repository.getAll().size()
            ) + 1;

    std::string newMemberId;


    while (true) {

        newMemberId =
            "M";

        if (nextNumber < 10) {

            newMemberId +=
                "00";

        }
        else if (nextNumber < 100) {

            newMemberId +=
                "0";
        }


        newMemberId +=
            std::to_string(
                nextNumber
            );


        if (
            repository.findById(
                newMemberId
            ) == nullptr
            ) {

            break;
        }


        nextNumber++;
    }


    member.memberId =
        newMemberId;


    if (member.status.empty()) {

        member.status =
            "ACTIVE";
    }


    return repository.add(
        member
    );
}


// =====================================
// LAY TAT CA THANH VIEN
// =====================================

const std::vector<Member>&
MemberService::getAllMembers() const {

    return repository.getAll();
}


// =====================================
// TIM THANH VIEN THEO ID
// =====================================

Member* MemberService::getMemberById(
    const std::string& memberId
) {

    if (memberId.empty()) {

        return nullptr;
    }


    return repository.findById(
        memberId
    );
}


// =====================================
// CAP NHAT THANH VIEN
// =====================================

bool MemberService::updateMember(
    const Member& member
) {

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


    Member updatedMember =
        member;


    if (updatedMember.status.empty()) {

        updatedMember.status =
            "ACTIVE";
    }


    return repository.update(
        updatedMember
    );
}


// =====================================
// XOA THANH VIEN
// =====================================

bool MemberService::deleteMember(
    const std::string& memberId
) {

    if (memberId.empty()) {

        return false;
    }


    return repository.removeById(
        memberId
    );
}
