#include "MugArgs.h"
#include "MugCLA.h"
#include "Server.h"
#include "MugConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "ThorsLogging/ThorsLogging.h"

#include <iostream>
#include <fstream>
#include <csignal>
#include <unistd.h>

static int sendSignalToRunningMug(ThorsAnvil::ThorsMug::MugArgs const& arguments)
{
    std::ifstream pidStream(arguments.pidFile);
    if (!pidStream) {
        std::cerr << "Failed to open PID file: " << arguments.pidFile << "\n";
        return 1;
    }
    pid_t pid;
    pidStream >> pid;
    if (pidStream.fail() || pid <= 0) {
        std::cerr << "Invalid PID in file: " << arguments.pidFile << "\n";
        return 1;
    }

    int sig = 0;
    if (arguments.signalType == ThorsAnvil::ThorsMug::SignalFlag::Reload) {
        sig = SIGHUP;
    }
    if (arguments.signalType == ThorsAnvil::ThorsMug::SignalFlag::Stop) {
        sig = SIGTERM;
    }
    else
    {
        std::cerr << "Unknown signal command: " << ThorsAnvil::Serialize::jsonExporter(arguments.signalType) << "\n"
                  << "Valid commands: reload, stop\n";
        return 1;
    }

    if (kill(pid, sig) != 0) {
        std::cerr << "Failed to send signal to process " << pid << ": " << strerror(errno) << "\n";
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[])
{
    using ThorsAnvil::ThorsMug::MugCLA;
    using ThorsAnvil::ThorsMug::MugArgs;
    using ThorsAnvil::ThorsMug::MugConfig;
    using ThorsAnvil::ThorsMug::Server;
    using ThorsAnvil::ThorsMug::ServerMode;

    try
    {
        MugArgs     arguments;
        MugCLA      parser(std::vector<std::string_view>(argv, argv + argc), arguments);

        if (!arguments.silent) {
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "========================================================");
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "===================== Start ============================");
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "========================================================");
        }
        if (arguments.help) { /// TODO
            parser.displayHelp(argv[0]);
            return 0;
        }

        if (arguments.signalType != ThorsAnvil::ThorsMug::SignalFlag::NoSignal) {
            return sendSignalToRunningMug(arguments);
        }

        if (arguments.configPath.empty()) {
            parser.displayHelp(argv[0]);
            ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "No config file set. Not explicitly set and default ones don't exist");
        }
        if (!std::filesystem::exists(arguments.configPath)) {
            parser.displayHelp(argv[0]);
            ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "Specified config file does not exist. Config File: ", arguments.configPath);
        }

        {
            std::ofstream pidStream(arguments.pidFile);
            if (!pidStream) {
                ThorsLogAndThrowError(std::runtime_error, "ThorsAnvil::ThorsMug::Server", "main", "Failed to write PID file: ", arguments.pidFile.c_str());
            }
            pidStream << getpid();
        }

        ServerMode mode = arguments.silent ? ServerMode::Headless : ServerMode::Active;
        bool reload = false;
        do {
            using ThorsAnvil::Serialize::jsonImporter;
            using ThorsAnvil::Serialize::ParserConfig;
            std::ifstream   configStream(arguments.configPath);
            MugConfig   config;

            if (!(configStream >> jsonImporter(config, ParserConfig{}))) {
                parser.displayHelp(argv[0]);
                ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "Failed to load config file: ", arguments.configPath);
            }

            if (config.servers.empty()) {
                parser.displayHelp(argv[0]);
                ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "Config specifies no servers to run");
            }

            Server       server(config, mode);
            server.run();
            reload = server.reloadRequested();
            if (reload) {
                ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "main", "Reload requested, re-reading config");
            }
        } while (reload);

        std::filesystem::remove(arguments.pidFile);

        if (!arguments.silent) {
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "========================================================");
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "====================== End =============================");
            ThorsLogInfo("ThorsAnvil::ThorsMug::Server", "Server", "========================================================");
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
        throw;
    }
    catch (...)
    {
        std::cerr << "Exception: Unknown\n";
        throw;
    }
}
