#include "City.hpp"

using namespace std;

City::City(string lon, string lat, string displayName) {
    this->lon = lon;
    this->lat = lat;
    this->displayName = displayName;
}

void City::getAllCitiesFromCityName(string cityName) {
    CURL* curl;
    CURLcode res;
    std::string response;
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://nominatim.openstreetmap.org/search?q=" + cityName + "&format=json");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if (res == CURLE_OK) {
            cout << response;
        } 
    }
    cout << response;
}