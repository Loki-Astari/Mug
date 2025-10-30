#include "ChaliceCLA.h"
#include "ThorSerialize/JsonThor.h"
#include "ThorsLogging/ThorsLogging.h"

#include <fstream>


using namespace ThorsAnvil::ThorsChalice;

std::vector<std::filesystem::path>        ChaliceCLA::searchPath = {"./chalice.cfg", "/etc/chalice.cfg", "/opt/homebrew/etc/chalice.cfg"};

SplitArg ChaliceCLA::splitArgument(std::string_view arg)
{
    std::size_t  find = arg.find('=');
    std::size_t  endF = find == std::string::npos ? arg.size() : find;
    std::size_t  argS = find == std::string::npos ? arg.size() : find + 1;

    std::string_view        flag(std::begin(arg), std::begin(arg) + endF);
    std::string_view        param(std::begin(arg) + argS, std::end(arg));

    return {flag, param};
}

void ChaliceCLA::parseArguments(std::vector<std::string_view> const& arguments)
{
    for (std::string_view const& arg: arguments)
    {
        SplitArg  argVal = splitArgument(arg);

        if (argVal.flag == "--help")
        {
            help = true;
            continue;
        }
        if (argVal.flag == "--silent")
        {
            silent = true;
            continue;
        }
        if (argVal.flag == "--logFile")
        {
            loguru::add_file(std::string(argVal.value).c_str(), loguru::FileMode::Truncate, loguru::g_stderr_verbosity);
            continue;
        }
        if (argVal.flag == "--logSys")
        {
            loguru::add_syslog(std::string(arguments[0]).c_str(), loguru::g_stderr_verbosity);
            continue;
        }
        if (argVal.flag == "--logLevel")
        {
            // Note: Default log level is loguru::Verbosity_3
            // See:  ThorsLogging/ThorsLogging.h
            if (argVal.value == "All") {
                loguru::g_stderr_verbosity = loguru::Verbosity_9;
            }
            else if (argVal.value == "Trace") {
                loguru::g_stderr_verbosity = loguru::Verbosity_8;
            }
            else if (argVal.value == "Debug") {
                loguru::g_stderr_verbosity = loguru::Verbosity_6;
            }
            else if (argVal.value == "Info") {
                loguru::g_stderr_verbosity = loguru::Verbosity_INFO;    // 0
            }
            else if (argVal.value == "Warn") {
                loguru::g_stderr_verbosity = loguru::Verbosity_WARNING; // -1
            }
            else if (argVal.value == "Error") {
                loguru::g_stderr_verbosity = loguru::Verbosity_ERROR;   // -2
            }
            // Note: There is no Fatal. This is always going to log and force application exit.
            else {
                help = true;
            }
            continue;
        }
        if (argVal.flag == "--config")
        {
            configPath = argVal.value;
            continue;
        }

        // Invalid Flag;
        help = true;
    }
}
void ChaliceCLA::displayHelp(std::string_view command)
{
    std::cout << "Usage: " << command << " [--help] [--silent] [--logLevel=(All|Trace|Debug|Info|Warn|Error)] [--config=<configFile>]\n"
              << R"(
--help:     prints out the help.
--silent:   does not spint startup information.
--logLevel: sets the logging level on all information sent via ThorsLogging.
--logFile:  add a file that log messages will be sent to.
--logSys:   send messages to syslog.
--config:   specifies a specific config file.

If no config file is explicitly specified then the following files are checked in order to see If they exist:
)";
    for (auto const& file: searchPath) {
         std::cout << "\t" << file << "\n";
    }

    std::cout << R"(
Chalice config is loaded from the config file.
It is an error when:
    * the config file does not exist
    * has an invalid format.
    * contains zero config objects

The server will refuse to start when there is an error.

)";

}

ChaliceCLA::ChaliceCLA(std::vector<std::string_view> const& arguments)
{
    parseArguments(arguments);

    if (configPath.empty())
    {
        for (auto path: searchPath)
        {
            if (std::filesystem::exists(path))
            {
                configPath = path;
                break;
            }
        }
    }

    using ThorsAnvil::Serialize::jsonImporter;
    std::ifstream   configStream(configPath);
    configStream >> jsonImporter(config);
}
