#ifndef MEMBERREPOSITORY_H
#define MEMBERREPOSITORY_H
#include <string>
#include <vector>
#include "../models/Member.h"
class MemberRepository {
private:
    std::vector<Member> members;
public:
    MemberRepository();
    std::vector<Member>& getAll();
    const std::vector<Member>& getAll() const;
    Member* findById(const std::string& memberId);
    const Member* findById(const std::string& memberId) const;
    bool add(const Member& member);
    bool update(const Member& member);
    bool removeById(const std::string& memberId);
};
#endif