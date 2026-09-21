#pragma once
#include <string>
using namespace std;
struct Reservation {
	string reservationId;
	string memberId;
	string bookId;
	string reservationDate;
	string status;

};
string getCurrentTime();
