//
// Created by Jules on 15/02/2024.
//

#include "string"
#include <sstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include "WeatherData.h"

using namespace std;

#ifndef SKYINSIGHT_WEATHERAPICALLER_H
#define SKYINSIGHT_WEATHERAPICALLER_H


class WeatherApiCaller {
    private:
        string apiKey;

    public:
        WeatherApiCaller(string apiKey);
        string getApiKey();
        WeatherData getCityInfo(string cityName);
};


#endif //SKYINSIGHT_WEATHERAPICALLER_H
