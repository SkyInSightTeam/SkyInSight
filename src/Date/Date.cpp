#include "Date.hpp"
#include <string.h>
#include <stdexcept>
#include <vector>
#include <ctime>

/**
 * @brief Construct a new Date:: Date object with current day
 *
 */
Date::Date()
{
    time_t now = time(0);
    struct tm tstruct = *localtime(&now);

    this->day = tstruct.tm_mday;
    this->month = tstruct.tm_mon + 1;
    this->year = tstruct.tm_year + 1900;
}

Date::Date(int day, int month, int year)
{
    if(!isDateValid(day, month, year)) {
        throw std::invalid_argument("❌ Invalid date: Date is not valid ❌");
    }
    this->day = day;
    this->month = month;
    this->year = year;
}

bool Date::isDateValid(int day, int month, int year)
{
    if (year < 1)
        return false;

    if (month < 1 || month > 12)
        return false;

    switch (month)
    {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return day >= 1 && day <= 31;
    case 4: case 6: case 9: case 11:
        return day >= 1 && day <= 30;
    case 2:
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
        {
            return day >= 1 && day <= 29;
        }
        else
        {
            return day >= 1 && day <= 28;
        }
    default:
        return false;
    }
}

std::string Date::getStringDate()
{
    return std::to_string(this->day) + "/" + std::to_string(this->month) + "/" + std::to_string(this->year);
}