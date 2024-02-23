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

WeatherData WeatherApiCaller::getCityInfo(string cityName) {
    curlpp::Cleanup cleaner;

    // Create a request object
    curlpp::Easy request;

    // Set the request URL
    request.setOpt(curlpp::options::Url("http://api.weatherapi.com/v1/current.json"));

    // Set the request method to POST
    request.setOpt(new curlpp::options::Post(true));

    // Set the request body data if needed
    std::string postData = "key=" + apiKey + "&q=" + cityName;
    request.setOpt(new curlpp::options::PostFields(postData));

    // Create a stringstream to store the response
    std::stringstream responseStream;

    // Set the write stream option to the stringstream
    request.setOpt(new curlpp::options::WriteStream(&responseStream));

    // Perform the request
    request.perform();

    // Get the response from the stringstream
    std::string response = responseStream.str();

    // std::cout << "Response: " << response << std::endl;

    WeatherData data;
    data.parseJson(response);


    string output = "";

    output += "Region: " + data.getRegion() + "\n";
    output += "Country: " + data.getCountry() + "\n";
    output += "Current temperature: " + to_string(data.getCurrentTempC()) + "°C" + "\n";
    output += "Sky condition: " + data.getConditionText() + "\n";
    output += "Wind: " + to_string(data.getWindKph()) + "km/h" + "\n";
    output += "Humidity: " + to_string(data.getHumidity()) + "%" + "\n\n";
    return data;

}