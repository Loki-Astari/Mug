#include "Mug.h"
#include "MugArgs.h"
#include "MugCLA.h"
#include "MugServer.h"
#include "MugConfig.h"

#include "ThorSerialize/JsonThor.h"
#include "ThorsLogging/ThorsLogging.h"

#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
    using ThorsAnvil::ThorsMug::MugCLA;
    using ThorsAnvil::ThorsMug::MugArgs;
    using ThorsAnvil::ThorsMug::MugConfig;
    using ThorsAnvil::ThorsMug::MugServer;
    using ThorsAnvil::ThorsMug::MugServerMode;

    std::cout << "Mug\n";
    try
    {
        MugArgs     arguments;
        MugCLA      parser(std::vector<std::string_view>(argv, argv + argc), arguments);

        if (arguments.help) { /// TODO
            parser.displayHelp(argv[0]);
            return 0;
        }

        if (arguments.configPath.empty()) {
            parser.displayHelp(argv[0]);
            ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "No config file set. Not explicitly set and default ones don't exist");
        }
        if (!FS::exists(arguments.configPath)) {
            parser.displayHelp(argv[0]);
            ThorsLogAndThrowError(std::runtime_error, "ThorsMug", "main", "Specified config file does not exist. Config File: ", arguments.configPath);
        }

        using ThorsAnvil::Serialize::jsonImporter;
        using ThorsAnvil::Serialize::ParserConfig;
        using ThorsAnvil::Serialize::ParseType;
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

        MugServer       server(config, arguments.silent ? MugServerMode::Headless : MugServerMode::Active);
        server.run();
    }
    catch (std::exception const& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
        throw;
    }
    catch (...)
    {
        std::cout << "Exception: Uknowne\n";
        throw;
    }
}
