#include "MugCLA.h"

#include <charconv>

using namespace ThorsAnvil::ThorsMug;

const Paths             MugCLA::searchPath = {"./mug.cfg", "/etc/mug.cfg", "/opt/homebrew/etc/mug.cfg"};
const VerbosityMap      MugCLA::verbosity  = {
                                                    {"All",     loguru::Verbosity_9},
                                                    {"Trace",   loguru::Verbosity_7},
                                                    {"Track",   loguru::Verbosity_5},
                                                    {"Debug",   loguru::Verbosity_3},
                                                    {"Info",    loguru::Verbosity_INFO},
                                                    {"Warn",    loguru::Verbosity_WARNING},
                                                    {"Error",   loguru::Verbosity_ERROR},
                                             };

SplitArg MugCLA::splitArgument(std::string_view arg)
{
    std::size_t  find = arg.find('=');
    std::size_t  endF = find == std::string::npos ? arg.size() : find;
    std::size_t  argS = find == std::string::npos ? arg.size() : find + 1;
    bool         hasValue = find != std::string::npos;

    std::string_view        flag(std::begin(arg), std::begin(arg) + endF);
    std::string_view        param(std::begin(arg) + argS, std::end(arg));

    return {flag, param, hasValue};
}

void MugCLA::parseArguments(std::vector<std::string_view> const& arguments)
{
    bool first = true;

    for (std::string_view const& arg: arguments)
    {
        ThorsLogDebug("MugCLA", "parseArguments", "Check arguments ", arg);
        if (first) {
            first = false;
            continue;
        }
        SplitArg  const argVal = splitArgument(arg);
        ThorsLogDebug("MugCLA", "parseArguments", "Flag:  ", argVal.flag);
        ThorsLogDebug("MugCLA", "parseArguments", "Value: ", argVal.value);

        if (argVal.flag == "--help")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate Help");
            args.setHelp();
            continue;
        }
        if (argVal.flag == "--silent")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate Silent");
            args.setSilent();
            continue;
        }
        if (argVal.flag == "--logFile")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate LogFile");
            args.logAddFile(argVal.value);
            continue;
        }
        if (argVal.flag == "--logSys")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate LogSys");
            args.logAddSys(!argVal.hasValue ? arguments[0] : argVal.value);
            continue;
        }
        if (argVal.flag == "--logLevel")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate LogLevel");
            auto find = verbosity.find(argVal.value);
            if (find == std::end(verbosity)) {
                int value = 0;
                auto result = std::from_chars(argVal.value.data(), argVal.value.data() + argVal.value.size(), value);
                if (result.ec == std::errc::invalid_argument) {
                    args.setHelp();
                    continue;
                }
                args.logSetLevel(value);
            }
            else {
                // Note: Default log level is loguru::Verbosity_3
                // See:  ThorsLogging/ThorsLogging.h
                args.logSetLevel(find->second);
            }
            continue;
        }
        if (argVal.flag == "--config")
        {
            ThorsLogDebug("MugCLA", "parseArguments", "Activate Config");
            setConfig = true;
            args.setConfigFile(argVal.value);
            continue;
        }

        // Invalid Flag;
        ThorsLogDebug("MugCLA", "parseArguments", "Invalid Flag");
        args.setHelp();
    }
}

void MugCLA::displayHelp(std::string_view command, std::ostream& output)
{
    output << "Usage: " << command << " [--help] [--silent] [--logLevel=(All|Trace|Track|Debug|Info|Warn|Error)] [--config=<configFile>]\n"
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
         output << "\t" << file << "\n";
    }

    output << R"(
Mug config is loaded from the config file.
It is an error when:
    * the config file does not exist
    * has an invalid format.
    * contains zero config objects

The server will refuse to start when there is an error.

)";

}

MugCLA::MugCLA(std::vector<std::string_view> const& arguments, MugCLAInterface& args)
    : args(args)
{
    parseArguments(arguments);

    if (!setConfig)
    {
        for (auto path: searchPath)
        {
            if (std::filesystem::exists(path))
            {
                args.setConfigFile(path);
                break;
            }
        }
    }
}
