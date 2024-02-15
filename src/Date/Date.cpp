#include "Date.hpp"
#include <string.h>

/**
 * @brief Construct a new Date:: Date object with JC
 *
 */
Date::Date()
{
    this->day = 0;
    this->month = 0;
    this->year = 0;
}

Date::Date(int day, int month, int year)
{
    this->day = day;
    this->month = month;
    this->year = year;
}

Date::Date(std::vector<std::string> date)
{
    this->day = stoi(date[0]);
    this->month = stoi(date[1]);
    this->year = stoi(date[2]);
}

std::string Date::getStringDate()
{
    return std::to_string(this->day) + "/" + std::to_string(this->month) + "/" + std::to_string(this->year);
}