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

std::vector<std::vector<std::string>> WeatherApiCaller::getCityLike(std::string name, std::string appid) {
    std::string url = "http://api.openweathermap.org/geo/1.0/direct?q=" + name + "&limit=20&appid=" + appid;
    std::vector<std::vector<std::string>> cityCoordinates;

    try {
        curlpp::Cleanup cleaner;
        curlpp::Easy request;
        request.setOpt(curlpp::options::Url(url));
        std::stringstream responseStream;
        request.setOpt(new curlpp::options::WriteStream(&responseStream));

        request.perform();
        long http_code;
        curl_easy_getinfo(request.getHandle(), CURLINFO_RESPONSE_CODE, &http_code);
        if (http_code == 200) {
            std::string response = responseStream.str();

            rapidjson::Document document;
            document.Parse(response.c_str());

            if (document.IsArray()) {
                for (const auto& city : document.GetArray()) {
                    std::vector<std::string> currentCity;
                    std::string cityName = city["name"].GetString();
                    double lat = city["lat"].GetDouble();
                    double lon = city["lon"].GetDouble();
                    std::string country = city["country"].GetString();
                    std::string state = city["state"].GetString();
                    std::string coordinates = std::to_string(lat) + ", " + std::to_string(lon);
                    currentCity.push_back(cityName);
                    currentCity.push_back(coordinates);
                    currentCity.push_back(state + ", " + country);
                    cityCoordinates.push_back(currentCity);
                }
            }
        } else {
            throw std::runtime_error("HTTP request failed with code: " + std::to_string(http_code));
        }
    } catch (const curlpp::LibcurlRuntimeError& e) {
        std::cerr << "cURLpp runtime error: " << e.what() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error: " << e.what() << std::endl;
    }

    return cityCoordinates;
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