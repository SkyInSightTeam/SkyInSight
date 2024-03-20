//
// Created by Jules on 15/02/2024.
//

#include "string"
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "WeatherData.hpp"
#include "../Date/Date.hpp"

using namespace std;

#ifndef SKYINSIGHT_WEATHERAPICALLER_H
#define SKYINSIGHT_WEATHERAPICALLER_H

class WeatherApiCaller
{
private:
    string apiKey;
    static std::string BASE_URL;

public:
    WeatherApiCaller(string apiKey);
    WeatherData getDateCityInfo(std::string querry, Date *date);
    string getApiKey();
    WeatherData getCityInfo(string querry);
    WeatherData getCityInfoByIp();
    std::vector<std::vector<std::string>> getCityLike(std::string name, std::string appid);
};

#endif // SKYINSIGHT_WEATHERAPICALLER_H
