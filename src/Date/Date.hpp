#ifndef dateHPP

#define dateHPP

#include <iostream>
#include <string.h>

class Date 
{
    private:
        int day, month, year;
    
    public:
        Date();
        Date(std::vector<std::string>);
        Date(int, int, int);
        std::string getStringDate();
};

#endif