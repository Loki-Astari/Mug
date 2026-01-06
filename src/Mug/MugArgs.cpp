#include "MugArgs.h"
#include <string>

using namespace ThorsAnvil::ThorsMug;

void MugArgs::setHelp()
{
    help = true;
}

void MugArgs::setSilent()
{
    silent = true;
}

void MugArgs::setConfigFile(std::filesystem::path file)
{
    configPath = file;
}

void MugArgs::logAddFile(std::filesystem::path file)
{
    loguru::add_file(std::string(file).c_str(), loguru::FileMode::Truncate, ThorsLogLevelGet());
}

void MugArgs::logAddSys(std::string_view app)
{
    loguru::add_syslog(std::string(app).c_str(), ThorsLogLevelGet());
}

void MugArgs::logSetLevel(loguru::Verbosity level)
{
    ThorsLogLevelSet(level);
}
