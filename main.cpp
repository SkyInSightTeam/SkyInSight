#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "./src/WeatherApiCaller/WeatherApiCaller.hpp"
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

auto warning = [](std::string_view message)
{
    std::cerr << "❌ Error: " << message << " ❌\n";
};

auto failure = [](std::string_view message)
{
    std::cerr << "❌ Error: " << message << " ❌\n";
    exit(-1);
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

std::vector<std::string> parseCitiesFromJson(const std::string &jsonContent)
{
    rapidjson::Document doc;
    doc.Parse(jsonContent.c_str());

    std::vector<std::string> cities;
    if (doc.HasMember("cities") && doc["cities"].IsArray())
    {
        const rapidjson::Value &citiesArray = doc["cities"];

        for (rapidjson::SizeType i = 0; i < citiesArray.Size(); i++)
        {
            if (citiesArray[i].IsString())
            {
                cities.push_back(citiesArray[i].GetString());
            }
        }
    }
    else
    {
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
    auto currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm* localTime = std::localtime(&currentTime);
    std::stringstream dateStream;
    dateStream << localTime->tm_mday << "/" << (localTime->tm_mon + 1) << "/" << (localTime->tm_year + 1900);

    std::string dateString = dateStream.str();
    return dateString;
}


std::vector<Element> getWeatherCols(std::string strListFilter, WeatherData data)
{
    std::vector<Element> columns = {text(data.getLocationName()) | border};

    for (char letter : strListFilter)
    {
        if (letter == 't')
        {
            // Add a column for temperature
            columns.push_back(text(to_string(data.getCurrentTempC()) + "°C" + "\n") | border | flex);
        }
        // Check if the letter corresponds to weather
        else if (letter == 'w')
        {
            // Add a column for weather
            columns.push_back(text(data.getConditionText()) | border | flex);
        }
    }
    return columns;
}


void replaceLine(const string& filename, int lineNumber, const string& newLine) {
    ifstream inputFile(filename);
    vector<string> lines;
    string line;

    // Read all lines from the file into a vector
    while (getline(inputFile, line)) {
        lines.push_back(line);
    }

    inputFile.close();

    if (lineNumber < 1 || lineNumber > lines.size()) {
        cout << "Invalid line number" << endl;
        return;
    }

    lines[lineNumber - 1] = newLine;

    ofstream outputFile(filename);
    for (const string& updatedLine : lines) {
        outputFile << updatedLine << endl;
    }
    outputFile.close();

    cout << "Line replaced successfully" << endl;
}

int getNumberOfLines(const string& filename) {
    ifstream inputFile(filename);
    string line;
    int numberOfLines = 0;

    while (getline(inputFile, line)) {
        numberOfLines++;
    }
    inputFile.close();
    return numberOfLines;
}



int main(int argc, char **argv)
{

    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;

    Date* start = new Date();
    Date *end = nullptr;
    std::string strListFilter = "tw"; // For temp and weather by default
    bool isCitySet = true;


    std::vector<std::string> cities;
    std::string city;
    std::string apiKey;


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
            if (argv[i + 1] == NULL)
            {
                std::cout << "Invalid argument" << std::endl;
                print_usage();
                exit(0);
            }

            std::string citiesConfig = argv[++i];

            std::ifstream file(citiesConfig);
            if (!file.is_open())
            {
                failure("Failed to open file: " + citiesConfig);
                exit(-1);
            }

            std::stringstream buffer;
            buffer << file.rdbuf();
            cities = parseCitiesFromJson(buffer.str());
            continue;
        }
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--city"))
        {
            if (argv[i + 1] == NULL)
            {
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
            if (argv[i + 1] == NULL)
            {
                start = new Date(split(getStringCurrentDate(), "/"));
                continue;
            }
            start = new Date(split(argv[++i], "/"));
            continue;
        }
        else if (!strcmp(argv[i], "-f") || !strcmp(argv[i], "--filter"))
        {
            if (argv[i + 1] == NULL)
            {
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
        else if (!strcmp(argv[i], "-key"))
        {
            if (argv[i+1]==NULL && argv[i + 1][0] != '-') {
                failure("You need to put the api key after -key");
            }
            apiKey = argv[++i];
            replaceLine("config.txt", 1, apiKey);
            continue;
        }
        else if (!strcmp(argv[i], "-setcity")) {
            if (i + 1 < argc && argv[i + 1] && argv[i + 1][0] != '-') {
                city = argv[++i];
                std::cout << "Default city set to: " << city << std::endl;
                replaceLine("config.txt", 2, city);
                continue;
            }
        }
        else
        { // ALL OTHER ARGUMENT
            print_usage();
            std::string err = "Unknow argument : " + std::string(argv[i]);
            failure(err);
        }
    }

    WeatherData data;
    if (!cities.empty())
    {
        std::vector<Element> rows;
        for (std::string city : cities)
        {
            WeatherData data = weatherApiCaller.getCityInfo(city);
            std::vector<Element> columns = getWeatherCols(strListFilter, data);
            rows.push_back(hbox(columns));
        }
        if (!rows.empty()) {
        Element document = vbox(vbox(std::move(rows))| border);

        auto screen = Screen::Create(
            Dimension::Full(),       // Width
            Dimension::Fit(document) // Height
        );
        Render(screen, document);
        screen.Print();

    std::fstream configFile("config.txt");
    if (configFile.is_open()) {
        if (getNumberOfLines("config.txt") != 2){
            std::ofstream outputFile("config.txt", ios::trunc);
            outputFile.close();

            for (int i=0; i < 2; i++) {
                configFile << "0" << endl;
            }

            failure("Do not change the config file manualy");

        }
        std::string line;
        int currentLine = 0;
        while (std::getline(configFile, line)) {
            if (currentLine == 0) {
                if (line == "0") {
                    failure("Missing api key. \n\t-h for help");
                }
                apiKey = line;
            }
            else if (currentLine == 1) {
                if (line != "0") {
                    city = line;
                    isCitySet = true;
                }
            }
            currentLine++;
        }
        configFile.close();
    }
    else
    {
        failure("Missing config file. \n\t-h for help");
    }
    WeatherApiCaller weatherApiCaller(apiKey);

    WeatherData data;
    if (isCitySet) {
        data = weatherApiCaller.getDateCityInfo(city, start);

    }
    }
    else
    {
        if (isCitySet)
        {
            data = weatherApiCaller.getCityInfo(city);
        }
        else
        {
            data = weatherApiCaller.getCityInfoByIp();
        }

        // Start with the basic "Ville" column
        std::vector<Element> columns = getWeatherCols(strListFilter, data);
        // Now, use the dynamically constructed columns in the hbox
        Element document = hbox(columns);
        auto screen = Screen::Create(
            Dimension::Full(),       // Width
            Dimension::Fit(document) // Height
        );
        Render(screen, document);
        screen.Print();
    }
    return EXIT_SUCCESS;
}