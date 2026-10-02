
#ifndef MEMBERSERVICE_H

#define MEMBERSERVICE_H

#include <string>
#include <vector>

#include "../models/Member.h"
#include "../repositories/MemberRepository.h"

class MemberService {

public:

    // Ket qua xac thuc mat khau thanh vien.
    struct AuthResult {
        bool isSuccess;
        string message;
    };

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

    // Xac thuc Member_ID + mat khau.
    // Moi nghiep vu co the thao tac duoc yeu cau mat khau
    // deu phai di qua ham nay.
    AuthResult authenticate(
        const std::string& memberId,
        const std::string& password
    ) const;

    bool updateMember(
        const Member& member
    );

    bool deleteMember(
        const std::string& memberId
    );
};

#endif
