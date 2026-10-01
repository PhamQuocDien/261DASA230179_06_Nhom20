#ifndef ADMINSERVICE_H
#define ADMINSERVICE_H

#include <string>

class AdminService
{
public:
    bool authenticate(
        const std::string& idAdmin,
        const std::string& password
    ) const;
};

#endif