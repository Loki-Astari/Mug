#include "ChaliceArgs.h"

using namespace ThorsAnvil::ThorsChalice;

void ChaliceArgs::setHelp()
{
    help = true;
}

void ChaliceArgs::setSilent()
{
    silent = true;
}

void ChaliceArgs::setConfigFile(FS::path file)
{
    configPath = file;
}

void ChaliceArgs::logAddFile(FS::path file)
{
    loguru::add_file(std::string(file).c_str(), loguru::FileMode::Truncate, loguru::g_stderr_verbosity);
}

void ChaliceArgs::logAddSys(std::string_view app)
{
    loguru::add_syslog(std::string(app).c_str(), loguru::g_stderr_verbosity);
}

void ChaliceArgs::logSetLevel(loguru::Verbosity level)
{
    loguru::g_stderr_verbosity = level;
}
