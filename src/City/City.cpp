#include "City.hpp"
#include <string>
#include <vector>

City::City(std::string lon, std::string lat, std::string displayName) {
    this->lon = lon;
    this->lat = lat;
    this->displayName = displayName;
}

City::getDisplayName() {
    return this->displayName;
}

City::getAllCitiesFromCityName(std::string cityName) {
  CURL* curl;
  CURLcode res;
  std::string response;

  curl = curl_easy_init();
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, ("https://nominatim.openstreetmap.org/search?q=" + cityName + "&format=json").c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
      // Handle potential errors from CURL request
      return std::vector<City>(); // Return empty vector on error
    }

    // Parse the JSON response to create City objects (using your chosen library)
    std::vector<City> cities;
    // ... (JSON parsing logic here)
    return cities;
  }

  return std::vector<City>(); // Return empty vector on error
}