#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include "src/Date/Date.hpp"
#include <vector>

std::string PROGNAME = "SkyInSight";
std::string FILE_NAME = __FILE__;
std::string RELEASE = "Revision 0.1 | Last update 6 Feb 2024";
std::string AUTHOR = "\033[1mAubertin Emmanuel\033[0m";
std::string COPYRIGHT = "(c) 2024 " + AUTHOR + " from https://athomisos.fr";
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
              << PROGNAME << " by " << PROGNAME << std::endl
              << "\033[1mUsage: \033[0m" << FILE_NAME << " | [-h | --help] | [-v | --version] " << std::endl
              << "          -h | --help                     Help" << std::endl
              << "          -v | --version                  Version" << std::endl
              << "          -c | --city <name>              Name of the city" << std::endl
              << "          -d | --date <date>        Day that you want (Today by default)" << std::endl
              << "          -i | --interval <date> <date>   Days that you want (Today by default)" << std::endl
              << std::endl
              << std::endl
              << "Date format: DD/MM/YYYY" << std::endl;
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

int main(int argc, char **argv)
{
    std::cout << "🤗  | Welcome in \033[1m" << PROGNAME << "\033[0m | 🤗" << std::endl;
    print_release();
    std::cout << std::endl
              << std::endl;

    std::string city = "";
    Date *start = nullptr;
    Date *end = nullptr;

    if (argc < 1) // number of arg minimum
        failure("One argument required. \n\t-h for help");

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
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--city"))
        {
            city = argv[++i];
            continue;
        }
        else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--city"))
        {
            city = argv[++i];
            std::cout << "Your city = " << city << std::endl;
            continue;
        }
        else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--date"))
        {
            start = new Date(split(argv[++i], "/"));
            continue;
        }
        else if (!strcmp(argv[i], "-i") || !strcmp(argv[i], "--interval"))
        {
            start = new Date(split(argv[++i], "/"));
            end = new Date(split(argv[++i], "/"));
            continue;
        }
        else
        { // ALL OTHER ARGUMENT
            print_usage();
            std::string err = "Unknow argument : " + std::string(argv[i]);
            failure(err);
        }
    }

    std::cout << "Input recap :" << std::endl
              << "\tCity : " << city << std::endl;
    if (end != nullptr)
    {
        std::cout << "\tDate : From " + start->getStringDate() + " To " + end->getStringDate() << std::endl;
    }
    else
    {
        std::cout << "\tDate : " + start->getStringDate() << std::endl;
    }
    
    return 0;
}
