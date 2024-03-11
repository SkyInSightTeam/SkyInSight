#include "string"
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <vector>
#include <iostream>

using namespace std;

class City {
    private:
        string lat;
        string lon;
        string displayName;

    public:
        City(string lat, string lon, string displayName);
        static void getAllCitiesFromCityName(string cityName);
};