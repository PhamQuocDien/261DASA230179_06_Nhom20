#ifndef DATE_H
#define DATE_H

#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Date {
    int year, month, day;

    int toDays() const {
        return year * 365 + month * 30 + day;
    }

    bool operator>(const Date& other) const { return this->toDays() > other.toDays(); }
    bool operator<(const Date& other) const { return this->toDays() < other.toDays(); }

    string toString() const {
        stringstream ss;
        ss << year << "-" << setfill('0') << setw(2) << month << "-" << setw(2) << day;
        return ss.str();
    }

    static Date parse(const string& s) {
        if (s.empty() || s == "null") return Date{0, 0, 0};
        Date d{2026, 1, 1};
        char c1, c2;
        stringstream ss(s);
        ss >> d.year >> c1 >> d.month >> c2 >> d.day;
        return d;
    }
};

#endif
