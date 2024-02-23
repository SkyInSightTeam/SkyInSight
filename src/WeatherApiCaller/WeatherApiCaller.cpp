//
// Created by Jules on 15/02/2024.
//

#include "WeatherApiCaller.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

WeatherApiCaller::WeatherApiCaller(string apiKey) {
    this->apiKey = apiKey;
}

string WeatherApiCaller::getApiKey() {
    return this->apiKey;
}

size_t WriteCallback(void *contents, size_t size, size_t nmemb, string *s) {
    size_t total_size = size * nmemb;
    s->append((char*)contents, total_size);
    return total_size;
}

WeatherData WeatherApiCaller::getCityInfo(string querry) {
    curlpp::Cleanup cleaner;

    curlpp::Easy request;
    request.setOpt(curlpp::options::Url("http://api.weatherapi.com/v1/current.json"));

    request.setOpt(new curlpp::options::Post(true));
    std::string postData = "key=" + apiKey + "&q=" + querry;
    request.setOpt(new curlpp::options::PostFields(postData));

    std::stringstream responseStream;
    request.setOpt(new curlpp::options::WriteStream(&responseStream));
    request.perform();

    long http_code;
    curl_easy_getinfo(request.getHandle(), CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        // Handle HTTP error here (e.g., throw an exception)
        throw std::runtime_error("HTTP request failed with code: " + std::to_string(http_code));
    }

    std::string response = responseStream.str();
    WeatherData data;
    data.parseJson(response);
    return data;
}