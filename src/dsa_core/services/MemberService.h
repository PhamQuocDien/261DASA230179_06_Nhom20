
#ifndef MEMBERSERVICE_H

#define MEMBERSERVICE_H

#include <string>
#include <vector>

#include "../models/Member.h"
#include "../repositories/MemberRepository.h"

class MemberService {

private:

    MemberRepository& repository;

public:

    explicit MemberService(
        MemberRepository& repository
    );

    bool addMember(
        Member& member
    );

    const std::vector<Member>&
        getAllMembers() const;

    Member* getMemberById(
        const std::string& memberId
    );

    bool updateMember(
        const Member& member
    );

    bool deleteMember(
        const std::string& memberId
    );
};

#endif
