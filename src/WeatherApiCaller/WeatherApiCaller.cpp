//
// Created by Jules on 15/02/2024.
//

#include "./WeatherApiCaller.hpp"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "../Date/Date.hpp"

std::string WeatherApiCaller::BASE_URL = "http://api.weatherapi.com/v1";
WeatherApiCaller::WeatherApiCaller(string apiKey)
{
    this->apiKey = apiKey;
}

string WeatherApiCaller::getApiKey()
{
    return this->apiKey;
}

size_t WriteCallback(char *buf, size_t size, size_t nmemb, std::string *response)
{
    response->append(buf, size * nmemb);
    return size * nmemb;
}

std::string GetExternalIP()
{
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if (res == CURLE_OK)
        {
            return response;
        }
        else
        {
            // Handle error
            std::cerr << "Failed to retrieve external IP: " << curl_easy_strerror(res) << std::endl;
        }
    }
    return "";
}

WeatherData WeatherApiCaller::getCityInfo(string querry)
{
    curlpp::Cleanup cleaner;

    curlpp::Easy request;
    request.setOpt(curlpp::options::Url(BASE_URL + "/current.json"));

    request.setOpt(new curlpp::options::Post(true));
    std::string postData = "key=" + apiKey + "&q=" + querry;
    request.setOpt(new curlpp::options::PostFields(postData));
    std::stringstream responseStream;
    request.setOpt(new curlpp::options::WriteStream(&responseStream));
    request.perform();

    long http_code;
    curl_easy_getinfo(request.getHandle(), CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200)
    {
        throw std::runtime_error("HTTP request failed with code: " + std::to_string(http_code));
    }

    std::string response = responseStream.str();
    WeatherData data;
    data.parseJson(response);
    return data;
}

WeatherData WeatherApiCaller::getDateCityInfo(std::string query, Date *date) {
    std::string endpoint = "/history.json";
    Date *currentDate = new Date();
    if (currentDate->formatDateAPI() == date->formatDateAPI()) {
        return getCityInfo(query);
    }
    if (*date > *currentDate) {
        endpoint = "/future.json";
    }
    std::string urlWithParams = BASE_URL + endpoint + "?key=" + apiKey + "&q=" + query + "&dt=" + date->formatDateAPI();
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    
    request.setOpt(new curlpp::options::Url(urlWithParams));  
    std::stringstream responseStream;
    request.setOpt(new curlpp::options::WriteStream(&responseStream));
    request.perform();
    long http_code;
    curl_easy_getinfo(request.getHandle(), CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        std::cerr << "HTTP request failed with code: " << std::to_string(http_code) << std::endl;
        std::cerr << "Response Body: " << responseStream.str() << std::endl;
        exit(-1);
    }
    std::string response = responseStream.str();
    WeatherData data;
    data.parseJson(response);
    return data;
}

WeatherData WeatherApiCaller::getCityInfoByIp()
{
    std::string externalIP = GetExternalIP();
    if (!externalIP.empty())
    {
        return getCityInfo(externalIP);
    }
    else
    {
        std::cerr << "Failed to retrieve external IP address." << std::endl;
    }
}