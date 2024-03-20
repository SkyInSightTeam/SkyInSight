#include <iostream>
#include <rapidjson/document.h>
#include <string>

class WeatherData
{
private:
    std::string locationName;
    std::string region;
    std::string country;
    double latitude;
    double longitude;
    std::string timezoneId;
    int localtimeEpoch;
    std::string localtime;

    double currentTempC;
    double currentTempF;
    bool isDay;
    std::string conditionText;
    std::string conditionIcon;
    int conditionCode;
    double windMph;
    double windKph;
    int windDegree;
    std::string windDir;
    double pressureMb;
    double pressureIn;
    double precipMm;
    double precipIn;
    int humidity;
    int cloud;
    double feelslikeC;
    double feelslikeF;
    double visibilityKm;
    double visibilityMiles;
    double uv;
    double gustMph;
    double gustKph;

    // Forecast details (assuming we're focusing on a single day's forecast)
    std::string forecastDate;
    double maxTempC;
    double minTempC;
    double maxTempF;
    double minTempF;
    double avgTempC;
    double avgTempF;
    double maxWindMph;
    double maxWindKph;
    double totalPrecipMm;
    double totalPrecipIn;
    double avgHumidity;
    double uvIndex;

public:
    void parseJson(const std::string &jsonString)
    {
        rapidjson::Document document;
        document.Parse(jsonString.c_str());

        if (document.HasMember("location"))
        {
            const auto &location = document["location"];
            if (location.IsObject())
            {
                locationName = location["name"].GetString();
                region = location["region"].GetString();
                country = location["country"].GetString();
                latitude = location["lat"].GetDouble();
                longitude = location["lon"].GetDouble();
                timezoneId = location["tz_id"].GetString();
                localtimeEpoch = location["localtime_epoch"].GetInt();
                localtime = location["localtime"].GetString();
            }
        }

        if (document.HasMember("current"))
        {
            const auto &current = document["current"];
            if (current.IsObject())
            {
                currentTempC = current["temp_c"].GetDouble();
                currentTempF = current["temp_f"].GetDouble();
                isDay = current["is_day"].GetInt() == 1;
                conditionText = current["condition"]["text"].GetString();
                conditionIcon = current["condition"]["icon"].GetString();
                conditionCode = current["condition"]["code"].GetInt();
                windMph = current["wind_mph"].GetDouble();
                windKph = current["wind_kph"].GetDouble();
                windDegree = current["wind_degree"].GetInt();
                windDir = current["wind_dir"].GetString();
                pressureMb = current["pressure_mb"].GetDouble();
                pressureIn = current["pressure_in"].GetDouble();
                precipMm = current["precip_mm"].GetDouble();
                precipIn = current["precip_in"].GetDouble();
                humidity = current["humidity"].GetInt();
                cloud = current["cloud"].GetInt();
                feelslikeC = current["feelslike_c"].GetDouble();
                feelslikeF = current["feelslike_f"].GetDouble();
                visibilityKm = current["vis_km"].GetDouble();
                visibilityMiles = current["vis_miles"].GetDouble();
                uv = current["uv"].GetDouble();
                gustMph = current["gust_mph"].GetDouble();
                gustKph = current["gust_kph"].GetDouble();
            }
        }
        else if (document.HasMember("forecast"))
        {
            const auto &forecastDay = document["forecast"]["forecastday"].GetArray()[0];

            forecastDate = forecastDay["date"].GetString();
            const auto &day = forecastDay["day"];
            maxTempC = day["maxtemp_c"].GetDouble();
            minTempC = day["mintemp_c"].GetDouble();
            maxTempF = day["maxtemp_f"].GetDouble();
            minTempF = day["mintemp_f"].GetDouble();
            currentTempC = day["avgtemp_c"].GetDouble();
            currentTempF = day["avgtemp_f"].GetDouble();
            maxWindMph = day["maxwind_mph"].GetDouble();
            maxWindKph = day["maxwind_kph"].GetDouble();
            totalPrecipMm = day["totalprecip_mm"].GetDouble();
            totalPrecipIn = day["totalprecip_in"].GetDouble();
            avgHumidity = day["avghumidity"].GetDouble();
            uvIndex = day["uv"].GetDouble();

            const auto &condition = day["condition"];
            conditionText = condition["text"].GetString();
            conditionIcon = condition["icon"].GetString();
            conditionCode = condition["code"].GetInt();
        }
    }

    std::string getLocationName() const { return locationName; }
    std::string getRegion() const { return region; }
    std::string getCountry() const { return country; }
    double getLatitude() const { return latitude; }
    double getLongitude() const { return longitude; }
    std::string getTimezoneId() const { return timezoneId; }
    int getLocaltimeEpoch() const { return localtimeEpoch; }
    std::string getLocaltime() const { return localtime; }

    double getCurrentTempC() const { return currentTempC; }
    double getCurrentTempF() const { return currentTempF; }
    bool getIsDay() const { return isDay; }
    std::string getConditionText() const { return conditionText; }
    std::string getConditionIcon() const { return conditionIcon; }
    int getConditionCode() const { return conditionCode; }
    double getWindMph() const { return windMph; }
    double getWindKph() const { return windKph; }
    int getWindDegree() const { return windDegree; }
    std::string getWindDir() const { return windDir; }
    double getPressureMb() const { return pressureMb; }
    double getPressureIn() const { return pressureIn; }
    double getPrecipMm() const { return precipMm; }
    double getPrecipIn() const { return precipIn; }
    int getHumidity() const { return humidity; }
    int getCloud() const { return cloud; }
    double getFeelslikeC() const { return feelslikeC; }
    double getFeelslikeF() const { return feelslikeF; }
    double getVisibilityKm() const { return visibilityKm; }
    double getVisibilityMiles() const { return visibilityMiles; }
    double getUv() const { return uv; }
    double getGustMph() const { return gustMph; }
    double getGustKph() const { return gustKph; }
};