#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "src/WeatherApiCaller/WeatherApiCaller.h"
#include "src/Date/Date.hpp"
#include <vector>
#include <ftxui/dom/elements.hpp>
#include <ftxui/screen/screen.hpp>
#include <chrono>

using namespace ftxui;
std::string PROGNAME = "SkyInSight";
std::string FILE_NAME = __FILE__;
std::string RELEASE = "Revision 0.1 | Last update 6 Feb 2024";
std::string AUTHOR = "\033[1mAubertin Emmanuel\033[0m";
std::string COPYRIGHT = "(c) 2024 " + AUTHOR + " from https://github.com/SkyInSightTeam";
bool VERBOSE = false;

auto print_release = []
{
    std::cout << RELEASE << '\n'
              << COPYRIGHT << '\n';
};

auto failure = [](std::string_view message)
{
    std::cerr << "❌ Error: " << message << " ❌\n";
};

void print_usage()
{
    std::cout << std::endl
              << PROGNAME << " by " << AUTHOR << std::endl
              << "\033[1mUsage: \033[0m" << PROGNAME << " | [-h | --help] | [-v | --version] " << std::endl
              << "          -h | --help                     Help" << std::endl
              << "          -v | --version                  Version" << std::endl
              << "          -c | --city <name>              Name of the city" << std::endl
              << "          -d | --date <date>              Day that you want (Today by default)" << std::endl
              << "          -i | --interval <date> <date>   Days that you want (Today by default)" << std::endl
              << "          -f | --filter <filter-list>     See filter usage for filter-list" << std::endl
              << std::endl
              << std::endl
              << "Date format: DD/MM/YYYY" << std::endl
              << "Filter usage:" << std::endl
              << "          ./SkyInsight --filter twp" << std::endl
              << "Filter avaible :" << std::endl
              << "          t       For temperature" << std::endl
              << "          w       For global weather (clear, rain...)" << std::endl;
}

auto print_help = []()
{
    print_release();
    std::cout << std::endl;
    print_usage();
    std::cout << std::endl
              << std::endl;
    exit(0);
};

std::vector<std::string> parseCitiesFromJson(const std::string& jsonContent) {
    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    std::vector<std::string> cities;
    if (doc.HasMember("cities") && doc["cities"].IsArray()) {
        const rapidjson::Value& citiesArray = doc["cities"];

        for (rapidjson::SizeType i = 0; i < citiesArray.Size(); i++) {
            if (citiesArray[i].IsString()) {
                cities.push_back(citiesArray[i].GetString());
            }
        }
    } else {
        std::cerr << "JSON does not contain 'cities' array." << std::endl;
    }

    return cities;
}

// for string delimiter
// source: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::string token;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
std::string getStringCurrentDate() {
    //Ce n'est pas ridiculement compliquer
    // Obtaining the current time
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

    // Convert the time to std::tm
    std::tm* localTime = std::localtime(&currentTime);

    // Create a stringstream to build the date string
    std::stringstream dateStream;

    // Insert the date components into the stringstream
    dateStream << localTime->tm_mday << "/" << (localTime->tm_mon + 1) << "/" << (localTime->tm_year + 1900);

    // Get the date string
    std::string dateString = dateStream.str();

    return dateString;
}

int main(int argc, char **argv)
{
    WeatherApiCaller weatherApiCaller("da044ffc858543249a5133512242302");

    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;

    Date *start = new Date();
    Date *end = nullptr;
    std::string strListFilter = "tw"; // For temp and weather by default
    bool isCitySet = true;

    std::vector<std::string> cities;
    std::string city;

    if (argc < 2)
    {
        std::ifstream configFile("config.txt");
        if (configFile.is_open())
        {
            std::string cityChose;
            if (std::getline(configFile, cityChose))
            {
                city = cityChose;
            }
            else
            {
                isCitySet = false;
            }
        }
        else
        {
            failure("One argument required. \n\t-h for help");
        }
    }

    for (int i = 1; i < argc; i++)
    {
        if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
        {
            print_usage();
            exit(0);
        }
        else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
        {
            print_release();
            exit(0);
        }
        else if (!strcmp(argv[i], "-C") || !strcmp(argv[i], "--Cities"))
        {
            if (argv[i+1]==NULL){
                std::cout << "Invalid argument" << std::endl;
                print_usage();
                exit(0);
            }

            std::string citiesConfig = argv[++i];

            std::ifstream file(citiesConfig);
            if (!file.is_open()) {
                failure("Failed to open file: " + citiesConfig );
                exit(-1);
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            cities = parseCitiesFromJson(buffer.str());
            exit(0);
        }
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--city"))
        {
            if (argv[i+1]==NULL){
                std::cout << "Invalid argument" << std::endl;
                print_usage();
                exit(0);
            }
            city = argv[++i];
            std::cout << "Your city = " << city << std::endl;
            isCitySet = true;
            continue;
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--date"))
        {
            if (argv[i+1]==NULL){
                start = new Date(split(getStringCurrentDate(), "/"));
                //std::cout << "Date par défaut, aujourd'hui: " <<start->getStringDate()<< std::endl;
                continue;
            }
            start = new Date(split(argv[++i], "/"));
            continue;
        }
        else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interval"))
        {
            if (argv[i+1]==NULL&&argv[i+2]==NULL){
                start = new Date(split(getStringCurrentDate(), "/"));
                end= new Date(split(getStringCurrentDate(), "/"));
                exit(0);
            }
            if (argv[i+1]==NULL||argv[i+2]==NULL){
                std::cout << "Argument invalide, veuillez préciser l'intervale souhaitée avec -i | --interval <date> <date>" << std::endl;
                exit(0);
            }
            start = new Date(split(argv[++i], "/"));
            end = new Date(split(argv[++i], "/"));
            continue;
        }
        else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--filter"))
        {
            if (argv[i+1]==NULL){
                std::cout << "Argument invalide, veuillez préciser un filtre avec --filter <filter-list>" << std::endl;
                exit(0);
            }
            strListFilter = argv[++i];
            continue;
        }
        else if (!strcmp(argv[i], "-ip"))
        {
            isCitySet = false;
            continue;
        }
        if (i + 1 < argc && argv[i + 1] && argv[i + 1][0] != '-') {
            city = argv[++i]; 
            std::cout << "Default city set to: " << city << std::endl;
            std::ofstream configFile("config.txt");
            if (configFile.is_open())
            {
                configFile << city;
                configFile.close();
            }
            else
            {
                std::cerr << "Error: Unable to open config.txt for writing." << std::endl;
            }
            continue;
        }
        else
        { // ALL OTHER ARGUMENT
            print_usage();
            std::string err = "Unknow argument : " + std::string(argv[i]);
            failure(err);
        }
    }

    WeatherData data;
    if(!cities.empty()){
        // TODO: Complete that part for interpretation.
    }
    if (isCitySet) {
        data = weatherApiCaller.getCityInfo(city);
    }
    else {
        data = weatherApiCaller.getCityInfoByIp();
    }

    // Start with the basic "Ville" column
    std::vector<Element> columns = {text(data.getLocationName()) | border};

    // Iterate over each character in strListFilter
    for (char letter : strListFilter) {
        // Check if the letter corresponds to temperature
        if (letter == 't') {
            // Add a column for temperature
            columns.push_back(text(to_string(data.getCurrentTempC()) + "°C" + "\n") | border | flex);
        }
        // Check if the letter corresponds to weather
        else if (letter == 'w') {
            // Add a column for weather
            columns.push_back(text(data.getConditionText()) | border | flex);
        }
    }

    // Now, use the dynamically constructed columns in the hbox
    Element document = hbox(columns);
    auto screen = Screen::Create(
        Dimension::Full(),       // Width
        Dimension::Fit(document) // Height
    );
    Render(screen, document);
    screen.Print();

    return EXIT_SUCCESS;
}