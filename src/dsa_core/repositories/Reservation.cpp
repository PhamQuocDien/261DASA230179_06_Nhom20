#include "Reservation.h"
#include<ctime>
#include <iomanip>
#include <sstream>


string getCurrentTime() {
    time_t now = time(nullptr);
    tm localTime = *localtime(&now);

    stringstream ss;
    ss << put_time(&localTime, "%d/%m/%Y %H:%M:%S");

    return ss.str();
}
