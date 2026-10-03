#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <string>

#include "../repositories/FineRepository.h"

struct PayFineResult {
    bool isSuccess;
    string message;
    string fineId;
    string status;
};

class AdminService
{
private:
    FineRepository& fineRepository;

public:
    explicit AdminService(
        FineRepository& fineRepository
    );

    bool authenticate(
        const std::string& idAdmin,
        const std::string& password
    ) const;

    // Xac nhan Admin da tra tien phat.
    // Xac thuc Admin_ID + mat khau o C++ truoc khi
    // doi trang thai Fine sang PAID.
    PayFineResult payFine(
        const std::string& fineId,
        const std::string& idAdmin,
        const std::string& password
    );
};

#endif