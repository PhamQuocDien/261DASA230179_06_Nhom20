#include "AdminService.h"

bool AdminService::authenticate(
    const std::string& idAdmin,
    const std::string& password
) const
{
    return
        idAdmin == "QuanLyAdmin" &&
        password == "QuanLyThuVien";
}