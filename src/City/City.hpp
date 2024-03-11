#ifndef cityHPP

#define cityHPP

#include <string>
#include <iostream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <vector>

using namespace std;

class City {
    private:
        string lat;
        string lon;
        string displayName;

    public:
        City(string lat, string lon, string displayName);
        vector<City> static getAllCitiesFromCityName(string cityName);
        string getDisplayName();
};

#endif