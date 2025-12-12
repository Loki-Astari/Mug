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
    loguru::add_file(std::string(file).c_str(), loguru::FileMode::Truncate, loguru::g_stderr_verbosity);
}

void MugArgs::logAddSys(std::string_view app)
{
    loguru::add_syslog(std::string(app).c_str(), loguru::g_stderr_verbosity);
}

void MugArgs::logSetLevel(loguru::Verbosity level)
{
    loguru::g_stderr_verbosity = level;
}
