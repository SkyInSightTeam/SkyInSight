#ifndef dateHPP

#define dateHPP

#include <iostream>
#include <string.h>
#include <vector>

class Date
{
private:
    int day, month, year;
    bool isDateValid(int, int, int);

public:
    Date();
    // Constructor that takes a vector of strings
    Date(std::vector<std::string> date) : Date(std::stoi(date[0]), std::stoi(date[1]), std::stoi(date[2])) {}

    Date(int, int, int);
    std::string getStringDate();
    std::string formatDateAPI() const;
    bool operator<(const Date &rhs) const;
    bool operator>(const Date &rhs) const;
    bool operator==(const Date &rhs) const;
    bool operator!=(const Date &rhs) const;
};

#endif