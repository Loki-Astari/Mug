#include "MugArgs.h"
#include <string>

using namespace ThorsAnvil::ThorsMug;

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::setHelp()
{
    help = true;
}

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::setSilent()
{
    silent = true;
}

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::setConfigFile(std::filesystem::path file)
{
    configPath = file;
}

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::logAddFile(std::filesystem::path file)
{
    loguru::add_file(std::string(file).c_str(), loguru::FileMode::Truncate, ThorsLogLevelGet());
}

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::logAddSys(std::string_view app)
{
    loguru::add_syslog(std::string(app).c_str(), ThorsLogLevelGet());
}

THORSMUG_HEADER_ONLY_INCLUDE
void MugArgs::logSetLevel(loguru::Verbosity level)
{
    ThorsLogLevelSet(level);
}
